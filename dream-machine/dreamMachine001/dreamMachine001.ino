/*
  An attempt to create Brion Gysin's Dreammachine device
  based on https://www.theguardian.com/artanddesign/2022/may/09/dreamachine-review-as-close-to-state-funded-psychedelic-drugs-as-you-can-get
  Inspiration by Adam Greenfield

  Flashing light between 8 and 13 flashes a second.
  Assumes 50% duty cycle

  created 9 May 2022
  by Tom Igoe
*/

int ledState = LOW;           // state of the light
const int ledPin = 2;         // output pin for light control
long lastChange = 0;          // timestamp for light change
// flashing rates, in ms:
const float fast = 1000 / 13;
const float slow = 1000 / 8;

void setup() {
  // set pin mode of the output pin:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read potentiometer:
  int knob = analogRead(A0);
  // map sensor values to flashing rates
  // at 50% duty cycle (/2):
  float interval = map(knob, 0, 1023, fast, slow) / 2;
  // if duty cycle interval has passed,
  // change light state:
  if (millis() - lastChange > interval) {
    digitalWrite(ledPin, ledState);
    // if HIGH, go LOW and vice versa:
    ledState = !ledState;
    // timestamp for next change:
    lastChange = millis();
  }
}
