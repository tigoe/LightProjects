/* Hue color temperature control example for ArduinoHttpClient library

   This example reads a TCS34725 light sensor to get the color temperature.
   Uses ArduinoHttpClient library to control Philips Hue
   For more on Hue developer API see http://developer.meethue.com

  Works with MKR1010, MKR1000, Nano 33 IoT
  Uses the following libraries:
   http://librarymanager/All#WiFi101  // use this for MKR1000
   http://librarymanager/All#WiFiNINA  // use this for MKR1010, Nano 33 IoT
   http://librarymanager/All#ArduinoHttpClient
   http://librarymanager/All#Arduino_JSON

  see readme.mfd file for more details.

  note: WiFi SSID and password are stored in arduino_secrets.h file.
  If it is not present, add a new tab, call it "arduino_secrets.h"
  and add the following defines, and change to your own values:

  #define SECRET_SSID "ssid"
  #define SECRET_PASS "password"

  created 5 april 2021
   by Tom Igoe
*/

#include <SPI.h>
//#include <WiFi101.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
// I2C and light sensor libraries:
#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;      // the Wifi radio's status
char hueHubIP[] = "192.168.0.4";  // IP address of the HUE bridge
String hueUserName = "Huebridgeusername"; // hue bridge username
int lightNumber = 5;

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, hueHubIP);
// change the values of these two in the arduino_serets.h file:
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
// set the content type:
const char contentType[] = "application/json";

// initialize the light sensor:
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// time of the last succcessful request to the Hue, in ms:
long lastRequestTime = 0;
int sendInterval = 60000;

// a JSON object for the body of the request:
JSONVar body;

void setup() {
  //Initialize serial and wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  if (!Serial) delay(3000);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    delay(2000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network IP = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  // If the client is not connected:
  if (!httpClient.connected()) {
    // and the request interval has passed:
    if (millis() - lastRequestTime >= sendInterval) {
      // read the sensor
      readSensor();
      // print latest reading, for reference:
      Serial.println(JSON.stringify(body));
      // make a String for the HTTP route:
      String route = "/api/" + hueUserName;
      route += "/lights/";
      route += lightNumber;
      route += "/state/";

      // make a PUT request:
      httpClient.put(route, contentType, JSON.stringify(body));
      // timestamp the request:
      lastRequestTime = millis();

    }
  }

  // If there is a response available, read it:
  if (httpClient.available()) {
    // read the status code and body of the response
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();

    // print out the response:
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: " );
    Serial.println(response);
    // close the request:
    httpClient.stop();
    // timestamp the request if you got a good response:
    if (statusCode == 200) {
      Serial.println("Good response");
    }
  }
}

void readSensor() {
  // get lux and color temperature from sensor:
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  Serial.println("CT: " + String(colorTemp));
  lux = tcs.calculateLux(r, g, b);
  // crude formula for lux conversion, based on a sensor reading
  // of 35000 in direct sunlight and 1500 in indirect sunlight.
  // lux is actually a power law curve.
  // TODO: get the proper conversion:
  float maxLux = 2000.0;
  lux = constrain(lux, 0, maxLux);
  Serial.println("lux: " + String(lux));
  // convert to a range from 0 - 254:
  int bri = lux / maxLux * 254;

  // convert color temp to mired value (see Hue API docs on ct property):
  // which is what the Hue expects:
  int mired = 1000000 / colorTemp;
  mired = constrain(mired, 153, 500);
  Serial.println(mired);
  // and map it to 153 to 500 for the Hue:

  // update elements of request body JSON object:
  body["bri"] = bri;
  body["on"] = true;
  body["ct"] = mired;
  body["transitiontime"] = sendInterval / 100;
}
