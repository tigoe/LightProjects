/*
  Sine fade
  Produces a sinusoidal fade curve

  created 30 Jan 2019
  modified 9 June 2019
  by Tom Igoe and lighting & interactivity 2019 class
*/
int currentLevel = 255;
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

    // map input to a 0-179 range:
    float angle = map(l, 0, maxValue, 0, 179);
    /* here's the explanation of the calulation:
      // convert to radians:
      float result = angle * PI/180;
      // now add PI/2 to offset by 90 degrees:
      result = result + PI/2;
      // get the sine of that:
      result = sin(result);
      // now you have -1 to 1. Add 1 to get 0 to 2:
      result = result + 1;
      // multiply to get 0-255:
      result = result * 127.5;
    */
    //here it all is in one line:
    float lightLevel = (sin((angle * PI / 180) + PI / 2) + 1) * 127.5;
    levelTable[l] = lightLevel;
  }
}
