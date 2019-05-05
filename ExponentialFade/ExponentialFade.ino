/*
  Exponential fade 
  Produces a fade on an exponential curve for dimming LEDs.
  Formula and explanation from 
  https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms

  created by Diarmuid Mac Namara
  adapted 5 May 2019
  by Tom Igoe
*/

int currentLevel = 1;
int change = 1;
const int maxLevel = 255;
float scalingFactor = 0.0;

void setup() {
  Serial.begin(9600);
  // Calculate the scaling factor based on the
  // number of PWM steps you want:
  scalingFactor = (maxLevel * log10(2)) / (log10(255));
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (currentLevel <= 0 || currentLevel >= maxLevel) {
    change = -change;
  }
  currentLevel += change;

  // calculate the light level:
  int lightLevel = pow(2, (currentLevel / scalingFactor)) - 1;

  //PWM output the result:
  analogWrite(5, lightLevel);
  delay(5);
  Serial.println(lightLevel);
}
