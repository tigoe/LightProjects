/*
  Exponential fade
  Produces a fade on an exponential curve for dimming LEDs.
  Formula and explanation from
  https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms

  created by Diarmuid Mac Namara
  adapted 5 May 2019
  by Tom Igoe
*/

int currentLevel = 1; // current light level
int change = 1;       // change each time you fade
byte levelTable[256]; // pre-calculated PWM levels

void setup() {
  Serial.begin(9600);
  // pre-calculate the PWM levels from exponent formula:
  fillLevelTable();
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= 255) {
    change = -change;
  }
  currentLevel += change;

  //PWM output the result:
  analogWrite(5, levelTable[currentLevel]);
  delay(5);
  Serial.println(levelTable[currentLevel]);
}

void fillLevelTable() {
  // set the range of values:
  float maxValue = 255;
  // Calculate the scaling factor based on the
  // number of PWM steps you want:
  float scalingFactor = (maxValue * log10(2)) / (log10(255));

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l <= maxValue; l++) {
    int lightLevel = pow(2, (l / scalingFactor)) - 1;
    levelTable[l] = lightLevel;
  }
}
