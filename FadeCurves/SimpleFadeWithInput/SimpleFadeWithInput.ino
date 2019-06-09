/*
 Linear Fade

 Takes input from an analog sensor and
 fades an LED on a linear path

 circuit:
  - LED attached to pin 5
  - 10Kilohm potentiometer attached to A0

 created by nearly everyone who's used an Arduino
 modified 9 June
 by Tom Igoe
 */

int brightness = 0;
int change = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
}
void loop() {
  // read potentiometer:
  int sensorReading = analogRead(A0);
  // map to 0-255 range:
  int currentLevel = map(sensorReading, 0, 1023, 0, 255);
  // change the light:
  analogWrite(5, currentLevel);
  Serial.println(currentLevel);
}
