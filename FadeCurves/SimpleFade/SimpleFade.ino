/*
 Linear Fade

 Fades an LED on a linear path

 created by nearly everyone who's used an Arduino
 modified 9 June
 by Tom Igoe
 */

int currentLevel = 0;
int change = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
}
void loop() {
  // add change to brightness, and constrain to 0-255:
  currentLevel = constrain(currentLevel + change, 0, 255);
  // if brightness is at either extreme, change the
  // direction of fading:
  if (currentLevel == 0 || currentLevel == 255) {
    change = -change;
  }
  // change the light:
  analogWrite(5, currentLevel);
  delay(10);
  Serial.println(currentLevel);
}
