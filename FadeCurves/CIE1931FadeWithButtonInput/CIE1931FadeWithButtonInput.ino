/*
   CIE1931 fade with pushbutton input

   Takes input from an analog sensor and produces an LED fade
   that appears visually linear, using the CIE1931
   perceived lightness formula.

   references:
   - http://hyperphysics.phy-astr.gsu.edu/hbase/vision/cie.html
   - https://jared.geek.nz/2013/feb/linear-led-pwm
   - https://github.com/lawtalker/rotary_dimmer/wiki
  circuit:
  - LED attached to pin 5
  - pushbuttons  attached to D2 and D3, wired as INPUT_PULLUP

  created 3 Dec 2025
  by Tom Igoe
*/

// number of steps = 2^PWM resolution:
const int steps = 256;
// change between steps:
int change = 1;
// current level:
int currentLevel = 0;
// pre-calculated PWM levels:
int levelTable[steps];

const int upButton = 3;
const int downButton = 2;
int lastUpButtonState = HIGH;
int lastDownButtonState = HIGH;
int previousLevel = 0;
int debounceDelay = 15;

void setup() {
  Serial.begin(9600);
  // wait for serial monitor to open:
  if (!Serial)
    delay(3000);
  // pre-calculate the PWM levels from the formula:
  fillLevelTable();
  // initialize digital pin 5 as an output:
  pinMode(5, OUTPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
}

void loop() {
  // read buttons:

  int buttonState1 = digitalRead(upButton);
  // if the up button is LOW:
  if (buttonState1 == LOW) {
    // if the level is less than max, fade up
    currentLevel = min(currentLevel++, 255);
    // delay for debouncing:
    delay(debounceDelay);
  }

  int buttonState2 = digitalRead(downButton);
  // if the down button is LOW:
  if (buttonState2 == LOW) {
    // if the level is greater than min, fade down:
    currentLevel = max(currentLevel--, 0);
    delay(debounceDelay);
  }

  if (currentLevel != previousLevel) {
    Serial.print(currentLevel);
    Serial.print("\t");
    // PWM output the result:
    analogWrite(5, levelTable[currentLevel]);
    Serial.println(levelTable[currentLevel]);
    previousLevel = currentLevel;
  }
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