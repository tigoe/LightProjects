/*
  X-squared fade
  Takes a linear input and produces a fade curve
  based on the square of the input

  created 5 May 2019 
  modified 9 June 2019
  by Tom Igoe
*/
int currentLevel = 1;
int change = 1;
byte levelTable[256]; // pre-calculated PWM levels

void setup() {
  Serial.begin(9600);
  // pre-calculate the PWM levels from the formula:
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
  delay(10);
  Serial.println(levelTable[currentLevel]);
}


void fillLevelTable() {
  // set the range of values:
  float maxValue = 255;

  // iterate over the array and calculate the right value for it:
  for (int l = 0; l <= maxValue; l++) {
    // square the current value:
    float lightLevel = pow(l, 2);
    // map the result back to a 0-255 range:
    lightLevel = map(lightLevel, 0, 65535, 0, 255);
    levelTable[l] = lightLevel;
  }
}
