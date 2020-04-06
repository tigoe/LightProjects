/*
  Networked datalogging light sensor client

  Connects to a server using HTTPS and uploads data.
  Follows the API of Don Coleman's mySQL server for Connected Devices course:
  https://github.com/don/itp-connected-devices
  Works with MKR1010, MKR1000, ESP8266.

  circuit:
  - AMS TCS34725 light sensor attached to SDA and SCL pins

  arduino_secrets.h file contains:
  #define SECRET_SSID ""        //  your network SSID (name)
  #define SECRET_PASS ""        // your network password
  #define SECRET_SESSIONKEY ""  // UUID for client in database
  #define SECRET_SERVER ""      // server IP

  created 5 Apr 2020
  by Tom Igoe
*/
// include appropriate libraries for your WiFi module:
#include <SPI.h>
//#include <WiFi101.h>      // for MKR1000 modules
#include <WiFiNINA.h>       // for MKR1010 modules
//#include <ESP8266WiFi.h>  // for ESP8266 modules

// include libraries for HTTP calls and JSON formatting:
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>

#include "Adafruit_TCS34725.h"  // light sensor library
#include <RTCZero.h>            // realtime clock library
#include "arduino_secrets.h"    // sensitive information

byte mac[6];              // mac address of your device
WiFiSSLClient netSocket;  // network socket to server
String route = "/data";   // API route
JSONVar sensorData;       // JSON for sensor data
JSONVar requestBody;      // JSOn for body of the request
String contentType = "application/json";  // content type header

// the HTTP client is global so you can use it in multiple functions below:
HttpClient client(netSocket, SECRET_SERVER, 443);
// initialize the sensor:
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
// initialize the realtime clock:
RTCZero rtc;

void setup() {
  Serial.begin(9600);              // initialize serial communication
  pinMode(LED_BUILTIN, OUTPUT);
  //  while (!Serial);
  // start sensor:
  while (!tcs.begin()) {
    Serial.println("Looking for sensor");
    delay(100);
  }
  // start RTC:
  rtc.begin();
  // turn network connection LED off:
  digitalWrite(LED_BUILTIN, LOW);
  connectToNetwork();
}

void loop() {
  // read sensor:
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  sensorData["sensor"] = "AMS TCS34725";
  sensorData["lux"] =  tcs.calculateLux(r, g, b);
  sensorData["CCT"] =  tcs.calculateColorTemperature_dn40(r, g, b, c);

  // every 5 minutes on the 0 second, send data to the server:
  if ((rtc.getMinutes() % 5 == 0) && (rtc.getSeconds() == 0)) {
    // add the sensor data to the request body:
    requestBody["data"] = JSON.stringify(sensorData);
    Serial.println(JSON.stringify(requestBody));
    // make the request:
    client.post(route, contentType, requestBody);

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: " );
    Serial.println(response);
    // when there's nothing left to the response,
    client.stop();           // close the request
  }
  // if disconnected from WiFi, attempt to reconnect:
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    connectToNetwork();
  }
}

void connectToNetwork() {
  // try to connect to the network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to: " + String(SECRET_SSID));
    //Connect to WPA / WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  Serial.println("connected to: " + String(SECRET_SSID));
  // You're connected, turn on the LED:
  digitalWrite(LED_BUILTIN, HIGH);

  // get your MAC address:
  WiFi.macAddress(mac);
  IPAddress ip = WiFi.localIP();
  Serial.print(ip);
  Serial.print("  MAC Address: ");
  Serial.print(macToString(mac));
  requestBody["macAddress"] = macToString(mac);
  requestBody["sessionKey"] = SECRET_SESSIONKEY;

  Serial.print("  Signal Strength: ");
  Serial.println(WiFi.RSSI());

  // set the time from the network:
  unsigned long epoch;
  do {
    Serial.println("Attempting to get network time");
    epoch = WiFi.getTime();
    delay(2000);
  } while (epoch == 0);

  rtc.setEpoch(epoch);
  Serial.println(String(rtc.getHours()) +
                 ":" + String(rtc.getMinutes())  +
                 ":" + String(rtc.getSeconds()));
}

// convert the MAC address to a String:
String macToString(byte mac[]) {
  String result;
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      result += "0";
    }
    result += String(mac[i], HEX);
    if (i > 0)  result += ":";
  }
  return result;
}
