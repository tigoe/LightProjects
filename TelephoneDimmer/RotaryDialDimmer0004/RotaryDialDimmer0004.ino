/*
  Uses a rotary phone to set the brightness of a Hue lamp

  Be sure to create an arduino_secrets.h tab with the following information:
#define SECRET_SSID ""
#define SECRET_PASS ""
#define SECRET_HUE_IP ""
#define SECRET_HUE_USER ""
#define HUE_LIGHT_NUMBER 47  // a light number
#define HUE_GROUP_NUMBER 1   // a group
#define HUE_SCENE_CODE "qJzKKuIkX7ey8lt"  // a scene for the group

  created 22 Jun 2019
  modified 28 Feb 2026
  by Tom Igoe
*/

#include <SPI.h>
#include <WiFi101.h>  // use this for the MKR1000 and WINC150x boards
//#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <ArduinoLowPower.h>  // note: works only on SAMD boards
#include "arduino_secrets.h"

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, SECRET_HUE_IP);
// JSON object for the HTTP request:
JSONVar lightState;
String command = "lights";
String action = "state";
int itemNumber = 1;

// inputs for the rotary phone. All are interrupts, change
// depending on the interrupt pins on your board:
const int rotaryPin = 0;     // changes when there's a dial happening
const int endRotaryPin = 1;  // goes high when dial is over
const int hookPin = 4;       // changes with the hook, and the dial
const int tonePin = 5;       // pin to make sound on the speaker

// status variables for the rotary phone:
volatile bool dialing = false;  // if dial is out of resting position
volatile int dialCount = 0;     // number dialed
bool hungUp = true;             // whether handset is on hook
const int debounceDelay = 12;   // 12 ms debounce for digital inputs
int sleepDelay = 10 * 1000;     // time until sleep
long lastDial = 0;              // time since last dial

void setup() {
  //Initialize serial:
  Serial.begin(9600);
  delay(2000);
  // configure the inputs for the rotary phone:
  pinMode(rotaryPin, INPUT_PULLUP);
  pinMode(endRotaryPin, INPUT_PULLUP);
  pinMode(hookPin, INPUT_PULLUP);

  // built in LED indicates network status:
  pinMode(LED_BUILTIN, OUTPUT);

  // set WiFi radio into max low power mode:
  WiFi.maxLowPowerMode();

  // attach interrupts for phone inputs:
  attachInterrupt(digitalPinToInterrupt(rotaryPin), count, FALLING);
  attachInterrupt(digitalPinToInterrupt(endRotaryPin), dial, CHANGE);
  // When phone goes off hook, processor should wake up:
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(hookPin), wakePhone, RISING);
}

void loop() {
  // LED shows whether the WiFi is connected or not:
  digitalWrite(LED_BUILTIN, (WiFi.status() == WL_CONNECTED));
  // read the hook pin:
  hungUp = digitalRead(hookPin);

  // take a timestamp when each dialing ends:
  if (digitalRead(endRotaryPin) == 0) {
    lastDial = millis();
  }
  // if the sleep delay has passed since the last dial,
  // and you're hung up, put the phone to sleep:
  if (millis() - lastDial > sleepDelay && hungUp) {
    sleepPhone();
  }

  // make a tone every 5 seconds while the
  // phone is off the hook:
  if (millis() % 5000 < 10 && !hungUp) {
    tone(tonePin, 440, 100);
    delay(20);
    tone(tonePin, 300, 100);
    delay(20);
    noTone(tonePin);
  }

  // if not hung up and not on the network,
  // try to connect:
  if (!hungUp && WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // if dial is at rest and there is a recent dial count,
  // then you haven't acted on the last dialed number:
  if (dialCount > 0 && digitalRead(endRotaryPin)) {
    if (Serial) Serial.println("Dial count: " + String(dialCount));
    // if you have a legitimate dial count:
    switch (dialCount) {
      case 10:
        // turn on the living room:
        command = "groups";
        action = "action";
        itemNumber = HUE_GROUP_NUMBER;
        lightState["scene"] = HUE_SCENE_CODE;
        break;
      case 1:
        // turn off the Sputnik:
        command = "lights";
        action = "state";
        itemNumber = HUE_LIGHT_NUMBER;
        lightState["on"] = false;
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        // dim the Sputnik:
        command = "lights";
        action = "state";
        itemNumber = HUE_LIGHT_NUMBER;
        lightState["on"] = true;
        int brightness = (dialCount - 1) * 31;
        lightState["bri"] = brightness;
        break;
    }
    // if there's a network connection,
    // and you're not hung up, send your request:
    if ((WiFi.status() == WL_CONNECTED)) {
      sendRequest(command, action, itemNumber, lightState);
    }
    // reset dial counter:
    dialCount = 0;
  }
}

void sendRequest(String cmd, String action, int item, JSONVar thisState) {
  // make a String for the HTTP request path:
  String request = "/api/" + String(SECRET_HUE_USER);
  request += "/";
  request += cmd;
  request += "/";
  request += item;
  request += "/";
  request += action;
  request += "/";
  String contentType = "application/json";

  // make a string for the JSON command:
  String hueCmd = JSON.stringify(thisState);
  // see what you assembled to send
  // (for debugging purposes only):
  if (Serial) Serial.print("PUT request to server: ");
  if (Serial) Serial.println(request);
  if (Serial) Serial.print("JSON command to server: ");
  if (Serial) Serial.println(hueCmd);

  // make the PUT request to the hub:
  httpClient.beginRequest();
  httpClient.put(request, contentType, hueCmd);
  httpClient.sendHeader("Connection", "close");
  httpClient.endRequest();

  // see if the server returned a 200 OK
  // the method below is faster than using the httpClient
  // methods:
  int statusCode = -1;
  bool success = false;
  // while client is connected,
  while (httpClient.connected()) {
    // if there's any incoming data:
    if (httpClient.available()) {
      // look for HTTP/1.1 :
      if (httpClient.findUntil("HTTP/1.1", " ")) {
        // the status code follows that:
        statusCode = httpClient.parseInt();
        // if it's 200, look for the "success" in the body:
        if (statusCode == 200) {
          if (httpClient.find("success")) {
            success = true;
          }
        }
      }
    }
  }
  if (Serial) Serial.println(statusCode);

  if (statusCode == 200 && success) {
    // beep to indicate success
    tone(tonePin, 440, 100);
    delay(100);
    noTone(tonePin);
  }
  if (statusCode < 0) {
    // beep twice to indicate failure:
    tone(tonePin, 300, 100);
    delay(120);
    tone(tonePin, 300, 100);
    delay(120);
    noTone(tonePin);
  }
}

void connectToWiFi() {
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    // beep the line to indicate you're trying:
    tone(tonePin, 440, 100);
    delay(20);
    tone(tonePin, 350, 100);
    delay(20);
    if (Serial) Serial.print("Attempting to connect to SSID: ");
    if (Serial) Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  if (Serial) Serial.println("connected");
}

//  Interrupt handlers:

// handler for handset cradle input. Runs on processor
// wakeup.
void wakePhone() {
  if (Serial) Serial.println("waking up");
}

void sleepPhone() {
  if (Serial) Serial.println("Going to sleep");
  // disconnect the WiFi:
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
  // // turn off the status LED:
  digitalWrite(LED_BUILTIN, LOW);
  // sleep:
  LowPower.sleep();
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
    noTone(tonePin);
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