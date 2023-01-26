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
  modified 25 Jan 2023
  by Tom Igoe
*/


// analogWrite resolution (can be 10 for SAMD boards, has to be 8 for Uno):
const int resolution = 10;
// number of steps = 2^resolution:
const int steps = pow(2, resolution);
// change between steps:
int change = 1;
// current level:
int currentLevel = 1;
// pre-calculated PWM levels:
int levelTable[steps];

void setup() {
  Serial.begin(9600);
  // wait for serial monitor to open:
  if (!Serial) delay(3000);
  // pre-calculate the PWM levels from the formula:
  fillLevelTable();
  // set the analogWrite resolution:
  analogWriteResolution(resolution);
  // initialize digital pin 5 as an output:
  pinMode(5, OUTPUT);
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= steps - 1) {
    change = -change;
  }
  currentLevel += change;

  //PWM output the result:
  analogWrite(5, levelTable[currentLevel]);
  delay(10);
  Serial.println(levelTable[currentLevel]);
}

void fillLevelTable() {
  /*
    For CIE1931, the following formulas have  Y as luminance, and
    Yn is the luminance of a white reference (basically, max luminance).
    This assumes a perceived lightness value L between 0 and 100,
    and  a luminance value (Y) of 0-1.0.
    if L > 8:  Y = ((L + 16) / 116)^3 * Yn
    if L <= 8: Y = L * 903.3 * Yn
  */
  // scaling factor to convert from 0-100 to 0-steps:
  float scalingFactor = 100.0 / float(steps);
  // luminance value:
  float luminance = 0.0;

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l < steps; l++) {
    // you need to scale lightness from a 0-steps range to a 0-100 range:
    float lightness = float(l) * scalingFactor;
    if (lightness <= 8) {
      luminance = (lightness / 903.3);
    } else {
      luminance = (lightness + 16) / 116.0;
      luminance = pow(luminance, 3);
    }
    // multiply to get 0 to steps, and fill in the table:
    levelTable[l] = int(luminance * steps);
  }
}
