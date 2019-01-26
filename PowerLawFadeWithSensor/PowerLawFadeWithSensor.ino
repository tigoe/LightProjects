/*
  Sine fade
  Takes a linear input from an analog sensor
  and produces a sinusoidal fade curve

  created 25 Jan 2019
  by Tom Igoe
*/
void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorReading = analogRead(A0);
  float intensity = logFade(sensorReading, 10);
  Serial.print(sensorReading);
  Serial.print(",");
  Serial.println(intensity);
  // use the result to fade an LED:
  analogWrite(5, intensity);
}

float logFade(int inValue, int magnitude) {
  float val = inValue * magnitude /1024;
  //here it all is in one line:
  float result = pow(10, val);
  return result;
}
