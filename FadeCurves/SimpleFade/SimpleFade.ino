/*
 Linear Fade

 Fades an LED on a linear path
 created by nearly everyone who's used an Arduino
 
 to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command

 modified 31 May 2023
 by Tom Igoe
 */


// number of steps = 2^PWM resolution:
const int steps = 256;
// change between steps:
int change = 1;
// current level:
int currentLevel = 1;

void setup() {
  Serial.begin(9600);
  // wait for serial monitor to open:
  if (!Serial) delay(3000);
  // initialize digital pin 5 as an output:
  pinMode(5, OUTPUT);
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
