/*
*/
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
//#include <ArduinoLowPower.h>
#include "arduino_secrets.h"

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, SECRET_HUE_IP);

// inputs for the rotary phone:
const int rotaryPin = 5;
const int endRotaryPin = 7;

// status variables for the rotary phone:
volatile bool dialing = false;
volatile int dialCount = 0;
volatile bool requesting = false;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect.

  // attempt to connect to Wifi network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network IP = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // configure the inputs for the rotary phone:
  pinMode (rotaryPin, INPUT_PULLUP);
  pinMode (endRotaryPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(rotaryPin), count, FALLING);
  attachInterrupt(digitalPinToInterrupt(endRotaryPin), dial, RISING);
}

void loop() {
  if (!dialing && dialCount > 0) {
    Serial.println(dialCount);
    if (!requesting) {
      int brightness = 0;
      // 0 is off, 9 is brightest:
      if (dialCount < 10) {
        brightness = (dialCount + 1) * 25;
      }
      JSONVar hueLight;
      hueLight["bri"] = brightness;
      if (brightness > 0) {
        hueLight["on"] = true;
      } else {
        hueLight["on"] = false;
      }
      sendRequest(7, hueLight);   // turn light on
    }
    // reset digit counter:
    dialCount = 0;
  }
  //  } else {
  //    // put WiFi to sleep:
  //    // put processor to sleep:
  //    //  LowPower.sleep();
  //  }
}

void sendRequest(int light, JSONVar lightState) {
  requesting = true;
  digitalWrite(LED_BUILTIN, requesting);
  // make a String for the HTTP request path:
  String request = "/api/" + String(SECRET_HUE_USER);
  request += "/lights/";
  request += light;
  request += "/state/";

  String contentType = "application/json";

  // make a string for the JSON command:
  String hueCmd  = JSON.stringify(lightState);
  // see what you assembled to send:
  Serial.print("PUT request to server: ");
  Serial.println(request);
  Serial.print("JSON command to server: ");
  Serial.println(hueCmd);

  // make the PUT request to the hub:
  httpClient.put(request, contentType, hueCmd);

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Server response: ");
  Serial.println(response);
  Serial.println();
  requesting = false;
  digitalWrite(LED_BUILTIN, requesting);
}

void dial() {
  dialing = false;
}

void count() {
  dialing = true;
  // slight delay to debounce internal contacts:
  delayMicroseconds(11);
  if (digitalRead(rotaryPin) == LOW) {
    dialCount++;
  }
}
