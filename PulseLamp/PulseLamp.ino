/* Pulse sensor lamp dimmer

  Controls a lamp using the average heart rate detected from a
  MAX30101 particle sensor

  Uses the Sparkfun MAX 3010x sensor library:
  https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library
  HR code is based on the example 5 from that library.

  circuit:
  - MAX30301 sensor connected to SDA and SCL pins
  - Transistor connected to pin 3 to control a 12V DC lamp

  created 1 Dec 2020
  by Tom Igoe
*/

// include libraries:
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

const int lampPin = 3;
MAX30105 sensor;

//Number of heart rate samples to average for the average heart rate.
// increase this for a smoother rate, decrease for more responsivity.
/// 4 is a decent compromise between the two:
const byte SAMPLES = 4;
//Array of heart rates:
byte heartRates[SAMPLES];
// index for the array:
byte arrayIndex = 0;
float beatsPerMinute;
int avgBPM;
// last time in ms that you got a heartbeat:
long lastBeat = 0;

// light variables:
int level = 127;
// last time the light level was changed:
long lastChange = 0;
// direction of the light level fade:
int fadeChange = 1;

// pre-calculated PWM levels to produce a nicer fade curve.
// See https://tigoe.github.io/LightProjects/fading.html for other examples:
byte cie1931[256];

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");

  // Initialize sensor:
  while (!sensor.begin()) {
    Serial.println("Heart rate sensor was not found. Please check wiring/power. ");
    delay(1000);
  }
  // Configure sensor with default settings:
  sensor.setup();
  // Turn Red LED to low to indicate sensor is running:
  sensor.setPulseAmplitudeRed(0x0A);
  // Turn off Green LED:
  sensor.setPulseAmplitudeGreen(0);

  // initiate the I/O pin for the transistor that's controlling the lamp:
  pinMode(lampPin, OUTPUT);
  pinMode (LED_BUILTIN, OUTPUT);
  // fill the table of PWM values to produce a nice light fade curve:
  fillCIETable();
}

void loop() {
  byte intensity;

  // change the level of the light every (beatAverage /2) milliseconds:
  if (millis() - lastChange > avgBPM ) {
    // reverse direction of fade when level is 0 or 255:
    if (level == 0 || level == 255) {
      fadeChange = -fadeChange;
    }
    // change the level using the fadeChange value:
    level += fadeChange;
    // get the PWM level from the table:
    intensity = cie1931[level];
    // set the PWM pin with it:
    analogWrite(lampPin, intensity);
    lastChange = millis();
  }

  // read the sensor:
  long irValue = sensor.getIR();
  // if the IR level is too high, there's no finger there to sense.
  // you can skip the rest of the loop() and return to the top of the loop:
  if (irValue < 50000) {
    Serial.println(" put your index finger on the sensor");
    return;
  }

  // Once you have an IR value, check to see if it counts as a heart beat
  // (uses a function in heartRate.h):
  if (checkForBeat(irValue)) {
    // turn on built in LED each heartbeat:
    digitalWrite(LED_BUILTIN, HIGH);
    // get the duration of this beat:
    long beatDuration = millis() - lastBeat;
    //save current time for comparison next time:
    lastBeat = millis();
    // calculte BPM from beat duration:
    beatsPerMinute = 60 / (beatDuration / 1000.0);

    // if BPM is in a reasonable range for human heart rate (20 - 255?),
    // put it in the array for averaging:
    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      // Store this reading in the array:
      heartRates[arrayIndex++] = (byte)beatsPerMinute;
      // limit the array pointer to the size of the array:
      arrayIndex %= SAMPLES;

      //Take average of readings
      avgBPM = 0;
      for (byte x = 0 ; x < SAMPLES ; x++) {
        avgBPM += heartRates[x];
      }
      avgBPM = avgBPM / SAMPLES;
    }
  } else {
    // turn off built in LED if no  heartbeat:
    digitalWrite(LED_BUILTIN, LOW);
  }
  // print the intensity and heart rate data:
  Serial.print("Intensity: ");
  Serial.print(intensity);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.println(avgBPM);
}


void fillCIETable() {
  /*
    For CIE, the following formulas have  Y as luminance, and
    Yn is the luminance of a white reference (basically, max luminance).
    This assumes a perceived lightness value L* between 0 and 100,
    and  a luminance value Y of 0-1.0.
    if L* > 8:  Y = ((L* + 16) / 116)^3 * Yn
    if L* <= 8: Y = L* *903.3 * Yn
  */
  // set the range of values:
  float maxValue = 255;
  // scaling factor to convert from 0-100 to 0-maxValue:
  float scalingFactor = 100 / maxValue;
  // luminance value:
  float Y = 0.0;

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l <= maxValue; l++) {
    // you need to scale L from a 0-255 range to a 0-100 range:
    float lScaled = float(l) * scalingFactor;
    if ( lScaled <= 8 ) {
      Y = (lScaled / 903.3);
    } else {
      float foo = (lScaled + 16) / 116.0;
      Y = pow(foo, 3);
    }
    // multiply to get 0-maxValue, and fill in the table:
    cie1931[l] = Y * maxValue;
  }
}
