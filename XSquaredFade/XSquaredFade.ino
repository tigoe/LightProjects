/*
  X-squared fade
  Takes a linear input and produces a fade curve
  based on the square of the input

  created 5 May 2019
  by Tom Igoe
*/
int currentLevel = 1;
int change = 1;
void setup() {
  Serial.begin(9600);
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= 255) {
    change = -change;
  }
  currentLevel += change;

  // square the current value:
  float result = pow(currentLevel, 2);
  // map the result back to a 0-255 range:
  result = map(result, 0, 65535, 0, 255);

  //PWM output the result:
  analogWrite(5, result);
  delay(5);
  Serial.println(result);
}
