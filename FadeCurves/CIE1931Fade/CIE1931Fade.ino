/*
   CIE1931 fade
   Produces an LED fade that appears visually linear, using the CIE1931
   perceived lightness formula.

   references:
   - http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html
   - https://jared.geek.nz/2013/feb/linear-led-pwm
   - https://github.com/lawtalker/rotary_dimmer/wiki
  circuit:
  - LED attached to pin 5

  to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command
 
  created 5 May 2019
  modified 23 May 2022
  by Tom Igoe
*/

int currentLevel = 1; // current light level
int change = 1;       // change each time you fade
const int resolution = 10;
const int steps = pow(2, resolution);
int cie1931[steps];    // pre-calculated PWM levels

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(3000);
  analogWriteResolution(resolution);
  // pre-calculate the PWM levels from CIE1931 formulas:
  fillCIETable();
}

void loop() {

  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= steps) {
    change = -change;
  }
  currentLevel += change;

  // PWM output the result. Get levels from
  // the pre-calculated CIE1931 table:
  analogWrite(5, cie1931[currentLevel]);
  delay(10);
  // Serial.println(steps);
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
  float maxValue = steps;
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
