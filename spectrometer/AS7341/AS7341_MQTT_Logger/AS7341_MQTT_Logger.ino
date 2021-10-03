/*
  Datalogger MQTT client - JSON

  Connects to an MQTT broker and uploads data.
  Uses realtime clock on the SAMD21 (MKR boards and Nano 33 IoT) to
  keep time.

  Modified for AS7341 spectrometer.
  NOTE: this version is not doing any corrections on the readings,
  it's just using basic counts. 
  TODO: correct for physical setup, see 
  https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740

  Works with MKR1010, MKR1000, Nano 33 IoT
  Uses the following libraries:
   http://librarymanager/All#WiFi101  // use this for MKR1000
   http://librarymanager/All#WiFiNINA  // use this for MKR1010, Nano 33 IoT
   http://librarymanager/All#ArduinoMqttClient
   http://librarymanager/All#Arduino_JSON
   http://librarymanager/All#RTCZero
   http://librarymanager/All#Adafruit_AS7341 (for the sensor)

  created 13 Jun 2021
  modified 26 July 2021
  by Tom Igoe
*/
// include required libraries and config files
#include <SPI.h>
//#include <WiFi101.h>        // for MKR1000 modules
#include <WiFiNINA.h>  // for MKR1010 modules and Nano 33 IoT modules
// for simplifying JSON formatting:
#include <Arduino_JSON.h>
// realtime clock module on the SAMD21 processor:
#include <RTCZero.h>
#include <ArduinoMqttClient.h>
// I2C and light sensor libraries:
#include <Wire.h>
#include <Adafruit_AS7341.h>
#include "arduino_secrets.h"

// instance of the sensor library:
Adafruit_AS7341 as7341;
// array to hold the raw readings:
uint16_t readings[12];

// initialize WiFi connection using SSL
// (use WIFiClient and port number 1883 for unencrypted connections):
WiFiSSLClient wifi;
MqttClient mqttClient(wifi);
String addressString = "";
// the wavelengths of each channel
// with 0 for placeholder on redundant or non-numeric wavelength channels:
int wavelengths[] = { 415, 445, 480, 515, 0, 0, 555, 590, 630, 680, 0, 910 };

// details for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 8883;
char topic[] = "light-readings";
const char willTopic[] = "light-readings/will";
String clientID = "light-client-";
const char location[] = "AS7341_spectrometer";

// initialize RTC:
RTCZero rtc;
unsigned long startTime = 0;

// a JSON variable for the body of your requests:
JSONVar body;

// timestamp for the sensor reading and send:
long lastSendTime = 0;

// interval between requests, in minutes:
int sendInterval = 1;
// time before broker should release the will, in ms:
long int keepAliveInterval = sendInterval * 10 * 60 * 1000;
// number of successful readings that have been sent:
unsigned long readingCount = 0;

void setup() {
  Serial.begin(9600);  // initialize serial communication
  // if serial monitor is not open, wait 3 seconds:
  if (!Serial) delay(3000);
  // start the realtime clock:
  rtc.begin();
  // array for WiFi MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      // if the byte is less than 16, add a 0 placeholder:
      addressString += "0";
    }
    // add the hexadecimal representation of this byte
    // to the address string:
    addressString += String(mac[i], HEX);
  }

  // add the MAC address to the sensor as an ID:
  body["uid"] = addressString;
  // add the location:
  body["location"] = location;

  // set the credentials for the MQTT client:
  // set a will message, used by the broker when the connection dies unexpectantly
  // you must know the size of the message before hand, and it must be set before connecting
  String willPayload = addressString + " sensor died";
  bool willRetain = true;
  int willQos = 1;
  // add location name to the client:
  clientID += location;
  mqttClient.setId(clientID);
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);
  mqttClient.setKeepAliveInterval(keepAliveInterval);
  mqttClient.beginWill(willTopic, willPayload.length(), willRetain, willQos);
  mqttClient.print(willPayload);
  mqttClient.endWill();


  if (!as7341.begin()) {
    Serial.println("Sensor not found, check wiring");
    while (true)
      ;
  }
  // set integration time:
  as7341.setATIME(35);
  as7341.setASTEP(10000);
  as7341.setGain(AS7341_GAIN_256X);
  // start a new reading cycle:
  as7341.startReading();
}

void loop() {
  //if you disconnected from the network, reconnect:
  if (WiFi.status() != WL_CONNECTED) {
    connectToNetwork();
  }

  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting to broker");
    connectToBroker();
  } else {
    // If the client is not connected:  if (!client.connected()) {
    // and the request interval has passed:
    if (abs(rtc.getMinutes() - lastSendTime) >= sendInterval) {
      // read the sensor, leave the loop if you don't get
      // a reading:
      if (!readSensor()) return;
      // put in a String object to get the length for beginMessage:
      String reading = JSON.stringify(body);
      // print it:
      if (Serial) Serial.println(reading);
      // message properties:
      bool retained = false;  // should broker retain the message or not
      int qos = 2;            // quality of service
      bool dup = false;       // keep duplicates?
      mqttClient.beginMessage(topic, reading.length(), retained, qos, dup);
      // add the value:
      mqttClient.print(reading);
      // send the message:
      mqttClient.endMessage();
      // increment the reading count:
      readingCount++;
      // take note of the time you make your request:
      lastSendTime = rtc.getMinutes();
    }
  }
}
/*
  readSensor. You could replace this with any sensor, as long as
  you put the results into the body JSON object
*/
bool readSensor() {
  bool result = false;  // check to see if readings are done:
  if (as7341.checkReadingProgress()) {
    // if so, read them:
    as7341.getAllChannels(readings);
    // loop over the readings and print them out:
    for (int r = 0; r < 12; r++) {
      // skip 4 and 5 as they are repeats:
      if (r == 4 || r == 5) continue;
      // get the wavelength for this channel and
      // put the reading into that element of the JSON:
      String thisBand = String(wavelengths[r]) + "nm";
      if (r == 10) thisBand = "Clear";
      body[thisBand] = as7341.toBasicCounts(readings[r]);
    }
    // start a new reading cycle:
    as7341.startReading();

    // update elements of request body JSON object:
    body["timeStamp"] = rtc.getEpoch();
    body["uptime"] = rtc.getEpoch() - startTime;
    body["readingCount"] = readingCount;
    // you got a good result, return true:
    result = true;
  }
  return result;
}

void connectToNetwork() {
  // try to connect to the network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to: " + String(SECRET_SSID));
    //Connect to WPA / WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  Serial.println("connected.");

  // set the time from the network:
  unsigned long epoch;
  do {
    Serial.println("Attempting to get network time");
    epoch = WiFi.getTime();
    delay(2000);
  } while (epoch == 0);

  // set the RTC:
  rtc.setEpoch(epoch);
  if (startTime == 0) startTime = rtc.getEpoch();
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);
  Serial.print("  Signal Strength: ");
  Serial.println(WiFi.RSSI());
}

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MOTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you can proceed:
  mqttClient.subscribe(topic);
  // return that you're connected:
  return true;
}
