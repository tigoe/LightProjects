/*
  Exponential fade
  Produces a fade on an exponential curve for dimming LEDs.
  Formula and explanation from
  https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms

 to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command
 
  created by Diarmuid Mac Namara
  adapted 5 May 2019
  modified 31 May 2023
  by Tom Igoe
*/

// number of steps = 2^PWM resolution:
const int steps = 256;
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

  // Calculate the scaling factor based on the
  // number of PWM steps you want:
  float scalingFactor = (steps * log10(2)) / (log10(steps));

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l < steps; l++) {
    int lightLevel = pow(2, (l / scalingFactor)) - 1;
    levelTable[l] = lightLevel;
  }
}
