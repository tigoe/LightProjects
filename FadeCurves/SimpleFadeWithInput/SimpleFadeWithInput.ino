/*
  Linear Fade

  Takes input from an analog sensor and
  fades an LED on a linear path

  circuit:
  - LED attached to pin 5
  - 10Kilohm potentiometer attached to A0

  to change from 10-bit resolution to 8, change the resolution variable
  and comment out the analogWriteResolution() command

  created by nearly everyone who's used an Arduino
  modified 31 May 2023
  by Tom Igoe
*/

int brightness = 0;
int change = 1;
const int steps = 8;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  while (!Serial) delay(3000);
  pinMode(5, OUTPUT);
}
void loop() {
  // read potentiometer:
  int sensorReading = analogRead(A0);
  // map to 0-255 range:
  int currentLevel = map(sensorReading, 0, 1023, 0, steps);
  // change the light:
  analogWrite(5, currentLevel);
  Serial.println(currentLevel);
}
