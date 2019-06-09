/*
   CIE1931 fade
   Takes input from an analog sensor and produces an LED fade
   that appears visually linear, using the CIE1931
   perceived lightness formula.

   Additionally, a Kalman filter using the SimpleKalmanFilter library
   smoothes the sensor input.

   references:
   - http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html
   - https://jared.geek.nz/2013/feb/linear-led-pwm
   - https://github.com/lawtalker/rotary_dimmer/wiki
   - https://github.com/denyssene/SimpleKalmanFilter
  circuit:
  - LED attached to pin 5
  - 10Kilohm potentiometer attached to A0

  created 9 June 2019
  by Tom Igoe
*/

#include <SimpleKalmanFilter.h>

int currentLevel = 1; // current light level
int change = 1;       // change each time you fade
byte cie1931[256];    // pre-calculated PWM levels

SimpleKalmanFilter filter(2, 2, 0.01);


void setup() {
  Serial.begin(9600);
  // pre-calculate the PWM levels from CIE1931 formulas:
  fillCIETable();
}

void loop() {
  // read potentiometer:
  int sensorReading = analogRead(A0);

  // calculate the estimated value with Kalman Filter
  float estimate = filter.updateEstimate(sensorReading);

  // map to 0-255 range:
  int currentLevel = map(estimate, 0, 1023, 0, 255);

  // PWM output the result. Get levels from
  // the pre-calculated CIE1931 table:
  analogWrite(5, cie1931[currentLevel]);
  Serial.println(cie1931[currentLevel]);
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
