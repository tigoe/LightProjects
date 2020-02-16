/*
  Uses a rotary phone to set the brightness of a Hue lamp

  created 22 Jun 2019
  modified 15 Feb 2020
  by Tom Igoe
*/
#include <SPI.h>
//#include <WiFi101.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <ArduinoLowPower.h>
#include "arduino_secrets.h"

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, SECRET_HUE_IP);

// inputs for the rotary phone:
const int rotaryPin = 2;
const int endRotaryPin = 3;
const int cradleHookPin = 9;

bool hungUp = true;

// status variables for the rotary phone:
volatile bool dialing = false;
volatile int dialCount = 0;
const int debounceDelay = 12;

// status variable for the HTTP request?
volatile bool requesting = false;

unsigned long timeToSleep = 0;
unsigned long sleepDelay = 5 * 60 * 1000;
int wakeups = 0;
int prevWakeups = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(5000);
  
  // configure the inputs for the rotary phone:
  pinMode (rotaryPin, INPUT_PULLUP);
  pinMode (endRotaryPin, INPUT_PULLUP);
  pinMode (cradleHookPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(rotaryPin), count, FALLING);
  attachInterrupt(digitalPinToInterrupt(endRotaryPin), dial,  CHANGE);
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(cradleHookPin), wakePhone, RISING);
}

void loop() {

  if (wakeups != prevWakeups) {
    if (!Serial) Serial.begin(9600);
    delay(1000);
    Serial.println("I woke up!");
    prevWakeups = wakeups;
    // reset the sleep timer:
    timeToSleep = 0;
  }

  if (!dialing && dialCount > 0) {
    Serial.println(dialCount);
    if (!requesting) {
      int brightness = 0;
      // 1 is off, 10 is brightest:
      if (dialCount <= 10) {
        brightness = (dialCount - 1) * 25;
      }
      JSONVar hueLight;
      // have to send "on" before "bri":
      if (brightness > 0) {
        hueLight["on"] = true;
      } else {
        hueLight["on"] = false;
      }
      if ( hueLight["on"]) {
        hueLight["bri"] = brightness;
      }
      if  (WiFi.status() == WL_CONNECTED) {
        sendRequest(2, hueLight);   // turn light on
      }
    }
    // reset digit counter:
    dialCount = 0;
  }
  // check to see if the phone is hung up:
  if (!dialing) {
    hungUp = digitalRead(cradleHookPin);
  }

  // if you're connected to WiFi, turn the LED on:
  digitalWrite(LED_BUILTIN, (WiFi.status() == WL_CONNECTED));


  if (!hungUp) {
    if (WiFi.status() != WL_CONNECTED
        && WiFi.status() <= WL_NO_SHIELD) {
      Serial.println("Wifi status:" + String(WiFi.status()));
      connectToWiFi();
    }
  } else {
    // if the phone is hung up and the sleep timer is not set:
    if (timeToSleep == 0) {
      Serial.println("setting sleep timer");
      // set a timer until sleep:
      timeToSleep = millis() + sleepDelay;
    }
    // if the time has passed, sleep:
    if (millis() >= timeToSleep) {
      // disconnect the WiFi:
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
        Serial.println("Disconnected from WiFi");
      }
      Serial.println("going to sleep now");
      digitalWrite(LED_BUILTIN, LOW);
      LowPower.sleep();
    }
  }
}

void sendRequest(int light, JSONVar lightState) {
  requesting = true;
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
}

void connectToWiFi() {
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
}


//  Interrupt handlers:

void wakePhone() {
  wakeups++;
}
void dial() {
  delay(debounceDelay);
  dialing = !digitalRead(endRotaryPin);
  if (dialing) {
    dialCount = 0;
  }
}

void count() {
  delay(debounceDelay);
  if (digitalRead(rotaryPin) == LOW) {
    dialCount++;
  }
}
