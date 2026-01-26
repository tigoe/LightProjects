/*
  MQTT Client
  Sends a JSON string of spectrometer readings to public.cloud.shiftr.io
  Uses AS7341 spectrometer.

  Works with MKR1010, MKR1000, Nano 33 IoT
  Uses the following libraries:
   http://librarymanager/All#WiFi101  // use this for MKR1000
   http://librarymanager/All#WiFiNINA  // use this for MKR1010, Nano 33 IoT
   http://librarymanager/All#ArduinoMqttClient
   http://librarymanager/All#Adafruit_AS7341 (for the sensor)

  created 13 Jun 2021
  modified 4 Nov 2024
  by Tom Igoe
*/
// include required libraries and config files
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

// properties of the object (the channels of the sensor):
// added 0 for channels 4 and 5, which are not used
int wavelengths[] = { 415, 445, 480, 515, 0, 0, 555, 590, 630, 680, 910 };

// last time the client sent a message, in ms:
long lastTimeSent = 0;
// message sending interval:
int interval = 120 * 1000;

// initialize WiFi connection using SSL
// (use WIFiClient and port number 1883 for unencrypted connections):
WiFiClient wifi;
// WiFiClient wifi;
MqttClient mqttClient(wifi);
// details for MQTT client:
const char broker[] = "public.cloud.shiftr.io";
const int port = 1883;
// you should use more unique names than this:
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
  // once every interval, send a message:
  if (millis() - lastTimeSent > interval) {
    // if you got a good read, send it:
    if (readSensor()) {
      mqttClient.beginMessage(topic);
      mqttClient.print(readingString);
      // send the message:
      mqttClient.endMessage();
      Serial.println(readingString);
      // timestamp this message:
      lastTimeSent = millis();
    }
  }
}

boolean readSensor() {
  // clear the reading string:
  readingString = "";
  // check to see if readings are done
  //  checkReadingProgress() is non-blocking:

  float result = as7341.checkReadingProgress();
  // if there is a reading, do corrections and parse it
  // into readingString:
  if (result) {
    // clear readingString:
    readingString = "{";
    // correction factors:
    // TODO: This is crude, and could be better with matrix math.
    // from https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740
    // fig 10

    // Added in 0 for 4 and 5 since those channels are not used below.
    float corrections[] = { 3.20, 3.00, 2.07, 1.30, 0, 0, 1.07, 0.93, 0.78, 0.71 };

    // get the readings:
    as7341.getAllChannels(readings);
    // loop over the readings and print them out:
    for (int r = 0; r < 9; r++) {
      // skip 4 and 5 as they are repeats:
      if (r == 4 || r == 5) continue;
      float thisReading = as7341.toBasicCounts(readings[r]);
      // if this is one of the channels with a correction, apply it:
      thisReading = thisReading * corrections[r];
      // format: "frequency":value,
      readingString += "\"" + String(wavelengths[r]);
      readingString += "\":" + String(thisReading);
      readingString += ",";
    }
    readingString += "\"clear\":";
    readingString += String(as7341.toBasicCounts(readings[10]));
    readingString += ",\"NIR\":";
    readingString += String(as7341.toBasicCounts(readings[11]));
    // add flicker detection:
    readingString += ",\"flicker\":";
    int flickerHz = as7341.detectFlickerHz();
    readingString += String(flickerHz);
    // add in JSON decorations:
    readingString += "}";
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
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you can proceed:
  mqttClient.subscribe(topic);
  // return that you're connected:
  return true;
}
