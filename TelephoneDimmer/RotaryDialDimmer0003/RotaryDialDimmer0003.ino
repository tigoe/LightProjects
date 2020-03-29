/*
  Uses a rotary phone to set the brightness of a Hue lamp

  Be sure to create an arduino_secrets.h tab with the following information:
  #define SECRET_SSID ""
  #define SECRET_PASS ""
  #define SECRET_HUE_IP ""
  #define SECRET_HUE_USER ""
  #define HUE_LIGHT_NUMBER yourLightNumber

  created 22 Jun 2019
  modified 28 Mar 2020
  by Tom Igoe
*/

#include <SPI.h>
//#include <WiFi101.h>      // use this for the MKR1000 and WINC150x boards
#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <ArduinoLowPower.h>    // note: works only on SAMD boards
#include <RTCZero.h>
#include "arduino_secrets.h"

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, SECRET_HUE_IP);

// inputs for the rotary phone. All are interrupts, change
// depending on the interrupt pins on your board:
const int rotaryPin = 2;
const int endRotaryPin = 3;
const int hookPin = 9;

// status variables for the rotary phone:
volatile bool dialing = false;  // if dial is out of resting position
volatile int dialCount = 0;     // number dialed
bool hungUp = true;             // whether handset is on hook
const int debounceDelay = 12;   // 12 ms debounce for digital inputs
bool lastHookState = false;

// variables for RTC, for processor sleep functionality:
RTCZero rtc;
int timeToSleep = -1;   // minutes from hangup

void setup() {
  //Initialize serial:
  Serial.begin(9600);

  // configure the inputs for the rotary phone:
  pinMode (rotaryPin, INPUT_PULLUP);
  pinMode (endRotaryPin, INPUT_PULLUP);
  pinMode (hookPin, INPUT_PULLUP);

  // built in LED indicates network status:
  pinMode(LED_BUILTIN, OUTPUT);

  // attach interrupts for phone inputs:
  attachInterrupt(digitalPinToInterrupt(rotaryPin), count, FALLING);
  attachInterrupt(digitalPinToInterrupt(endRotaryPin), dial,  CHANGE);
  // When phone goes off hook, processor should wake up:
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(hookPin), wakePhone, RISING);
  // start the realtime clock:
  rtc.begin();
}

void loop() {
  // LED shows whether the WiFi is connected or not:
  digitalWrite(LED_BUILTIN, (WiFi.status() == WL_CONNECTED));

  // check to see if the phone is hung up:
  hungUp = digitalRead(hookPin);
  delay(debounceDelay);
  if (hungUp) {
    // turn off dialtone:
    noTone(A0);
    // set a timer until sleep (now + 1 minute)
    // if it's not already set:
    if (timeToSleep < 0) {
      timeToSleep = (rtc.getMinutes() + 1) % 60;
      // serial messages only print if port is open:
      if (Serial) Serial.println("sleep at: " + String(timeToSleep));
    }
  } else {
    // if not hung up and not on the network,
    // try to connect:
    if (WiFi.status() != WL_CONNECTED
        && WiFi.status() <= WL_NO_SHIELD) {
      connectToWiFi();
    }
  }

  // if the sleep timer has passed, go to sleep:
  if (hungUp && (rtc.getMinutes() == timeToSleep)) {
    if (Serial) Serial.println("Going to sleep");
    // disconnect the WiFi:
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.disconnect();
    }
    // turn off the status LED:
    digitalWrite(LED_BUILTIN, LOW);
    // sleep:
    LowPower.sleep();
  }

  // if dial is at rest and there is a recent dial count,
  // then you haven't acted on the last dialed number:
  if (!dialing) {
    // if you have a legitimate dial count:
    if (dialCount > 0 && dialCount <= 10) {
      // 1 is off, 10 is brightest.
      // map to a 0-255 range:
      int brightness = (dialCount - 1) * 25;

      // JSON object for the HTTP request:
      JSONVar lightState;
      // you have to send "on" before "bri":
      if (brightness > 0) {
        lightState["on"] = true;
        lightState["bri"] = brightness;
      } else {
        lightState["on"] = false;
      }
      // if there's a network connection,
      // and you're not hung up, send your request:
      if  ((WiFi.status() == WL_CONNECTED) && !hungUp) {
        sendRequest(HUE_LIGHT_NUMBER, lightState);
      }
      // reset dial counter:
      dialCount = 0;
    }
  }
}

void sendRequest(int light, JSONVar thisState) {
  // make a String for the HTTP request path:
  String request = "/api/" + String(SECRET_HUE_USER);
  request += "/lights/";
  request += light;
  request += "/state/";
  String contentType = "application/json";

  // make a string for the JSON command:
  String hueCmd  = JSON.stringify(thisState);
  // see what you assembled to send
  // (for debugging purposes only):
  if (Serial) Serial.print("PUT request to server: ");
  if (Serial) Serial.println(request);
  if (Serial) Serial.print("JSON command to server: ");
  if (Serial) Serial.println(hueCmd);

  // make the PUT request to the hub:
  httpClient.put(request, contentType, hueCmd);

  // you don't need to read the responses, these are here
  // for debugging purposes only. Once things are working
  // you can comment this section out to speed things up:

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  if (Serial) Serial.print("Status code from server: ");
  if (Serial) Serial.println(statusCode);
  if (Serial) Serial.print("Server response: ");
  if (Serial) Serial.println(response);
  if (Serial) Serial.println();
}

void connectToWiFi() {
  // attempt to connect to Wifi network:
  while ( WiFi.status() != WL_CONNECTED) {
    // beep the line to indicate you're trying:
    tone(A0, 440, 100);
    delay(30);
    tone(A0, 350, 100);
    delay(30);
    if (Serial) Serial.print("Attempting to connect to WPA SSID: ");
    if (Serial) Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(1500);
  }

  // set the time from the network:
  unsigned long epoch;
  do {
    // beep the line to indicate you're trying:
    tone(A0, 440, 100);
    delay(30);
    tone(A0, 350, 100);
    delay(30);
    if (Serial) Serial.println("Attempting to get network time");
    epoch = WiFi.getTime();
    delay(1500);
  } while (epoch == 0);
  // set the time with the epoch value you got from the network:
  rtc.setEpoch(epoch);
  // make a dialtone:
  tone(A0, 440);
  delay(30);
  tone(A0, 350);
  delay(30);
}


//  Interrupt handlers:

// handler for handset cradle input. Runs on processor
// wakeup. Resets sleep timer:
void wakePhone() {
  timeToSleep = -1;
}

// dial input handler. Determines whether the dial
// is moving or not:
void dial() {
  delay(debounceDelay);
  dialing = !digitalRead(endRotaryPin);
  // if there's a dial during a request,
  // the dialCount might not be reset,
  // so reset it here too:
  if (dialing) {
    // turn off dialtone when dialing begins:
    noTone(A0);
    dialCount = 0;
  }
}

// dial input count handler. Counts numbers while dialing:
void count() {
  delay(debounceDelay);
  if (digitalRead(rotaryPin) == LOW) {
    dialCount++;
  }
}
