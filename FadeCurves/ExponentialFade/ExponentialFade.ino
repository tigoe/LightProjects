/*
  Exponential fade
  Produces a fade on an exponential curve for dimming LEDs.
  Formula and explanation from
  https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms

 to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command
 
  created by Diarmuid Mac Namara
  adapted 5 May 2019
  modified 23 May 2022
  by Tom Igoe
*/

int currentLevel = 1; // current light level
int change = 1;       // change each time you fade
const int resolution = 10;
const int steps = pow(2, resolution);
int levelTable[steps]; // pre-calculated PWM levels

void setup() {
  Serial.begin(9600);
    while(!Serial) delay(3000);
  analogWriteResolution(resolution);
  // pre-calculate the PWM levels from the formula:
  fillLevelTable();
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= steps) {
    change = -change;
  }
  currentLevel += change;

  //PWM output the result:
  analogWrite(5, levelTable[currentLevel]);
  delay(10);
  Serial.println(levelTable[currentLevel]);
}

void fillLevelTable() {
  // set the range of values:
  float maxValue = steps;
  // Calculate the scaling factor based on the
  // number of PWM steps you want:
  float scalingFactor = (maxValue * log10(2)) / (log10(steps));

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l <= maxValue; l++) {
    int lightLevel = pow(2, (l / scalingFactor)) - 1;
    levelTable[l] = lightLevel;
  }
}
