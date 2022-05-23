/*
 Linear Fade

 Fades an LED on a linear path

 created by nearly everyone who's used an Arduino
 
 to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command

 
 modified 23 May 2022
 by Tom Igoe
 */

int currentLevel = 0;
int change = 1;
const int resolution = 10;
const int steps = pow(2, resolution);

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  analogWriteResolution(resolution);
}
void loop() {
  // add change to brightness:
  currentLevel = currentLevel + change;
  // and constrain to 0-255:
  currentLevel = constrain(currentLevel, 0, steps);
  // if brightness is at either extreme, change the
  // direction of fading:
  if (currentLevel == 0 || currentLevel == steps) {
    change = -change;
  }
  // change the light:
  analogWrite(5, currentLevel);
  delay(10);
  Serial.println(currentLevel);
}
