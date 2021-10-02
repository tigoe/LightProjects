/*

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
   http://librarymanager/All#Adafruit_AS7341 (for the sensor)

  created 13 Jun 2021
  modified 2 Oct 2021
  by Tom Igoe
*/
// include required libraries and config files
#include <SPI.h>
//#include <WiFi101.h>        // for MKR1000 modules
#include <WiFiNINA.h>  // for MKR1010 modules and Nano 33 IoT modules
#include <ArduinoMqttClient.h>
// I2C and light sensor libraries:
#include <Wire.h>
#include <Adafruit_AS7341.h>
#include "arduino_secrets.h"

// instance of the sensor library:
Adafruit_AS7341 as7341;
// array to hold the raw readings:
uint16_t readings[12];
String readingString = "";

// initialize WiFi connection using SSL
// (use WIFiClient and port number 1883 for unencrypted connections):
WiFiSSLClient wifi;
// WiFiClient wifi;
MqttClient mqttClient(wifi);
// details for MQTT client:
const char broker[] = "public.cloud.shiftr.io";
const int port = 8883;
const char topic[] = "spectrometer";
String clientID = "spectrometerClient";

void setup() {
  Serial.begin(9600);  // initialize serial communication
  // if serial monitor is not open, wait 3 seconds:
  if (!Serial) delay(3000);

  // set the credentials for the MQTT client:
  mqttClient.setId(clientID);
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);
  // initialize the sensor:
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
  // connect to WiFi:
  connectToNetwork();
}

void loop() {
  //if you disconnected from the network, reconnect:
  if (WiFi.status() != WL_CONNECTED) {
    connectToNetwork();
  }

  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    connectToBroker();
  } 

  // if you got a good read, send it:
  if (readSensor()) {
    mqttClient.beginMessage(topic);
    mqttClient.println(readingString);
    // send the message:
    mqttClient.endMessage();
    Serial.println(readingString);
  }
}

bool readSensor() {
  bool result = false;
  // clear reading string for new readings:  
  readingString = ""; 
  // check to see if readings are done:
  if (as7341.checkReadingProgress()) {
    // if so, read them:
    as7341.getAllChannels(readings);
    // loop over the readings and print them out:
    for (int r = 0; r < 12; r++) {
      // skip 4 and 5 as they are repeats:
      if (r == 4 || r == 5) continue;
      // get the wavelength for this channel and
      // put the reading into that element of the JSON:
      readingString += String(as7341.toBasicCounts(readings[r]));
      if (r < 11) readingString += ",";
    }
    // reading is good:
    result = true;
    // start a new reading cycle:
    as7341.startReading();
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
