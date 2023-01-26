/*
  X-squared fade
  Takes a linear input and produces a fade curve
  based on the square of the input

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
  // iterate over the array and calculate the right value for it:
  for (int l = 0; l <= steps; l++) {
    // square the current value:
    float lightLevel = pow(l, 2);
    // map the result back to the resolution range:
    lightLevel = map(lightLevel, 0, 65535, 0, steps-1);
    levelTable[l] = lightLevel;
  }
}
