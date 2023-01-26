/*
  Sine fade
  Produces a sinusoidal fade curve

  to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command
  
  created 30 Jan 2019
  modified 25 Jan 2023
  by Tom Igoe and lighting & interactivity 2019 class
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
  // iterate over the array and calculate the right value for it:
  for (int l = 0; l < steps; l++) {
    // map input to a 0-360 range:
    int angle = map(l, 0, steps, 0, 360);
    // convert to radians:
    float lightLevel = angle * PI / 180;
    // now subtract PI/2 to offset by 90 degrees, so yuu can start fade at 0:
    lightLevel -= PI / 2;
    // get the sine of that:
    lightLevel = sin(lightLevel);
    // now you have -1 to 1. Add 1 to get 0 to 2:
    lightLevel += 1;
    // multiply to get 0-255:
    lightLevel *= (steps - 1) / 2;
    // put it in the array:
    levelTable[l] = int(lightLevel);
  }
}
