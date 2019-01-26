/*
  MPX2102 reading

  Reads an analog input on pin 0, scales it a bit, 
  prints the result to the Serial Monitor.

  Circuit: 
  * MPX2102 attached to an LM358 op amp attached to pin A0. 
  * See circuit drawing for details

  created 15 Jan 2019
  by Tom Igoe
*/

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read the input, subrtact half the range:
  int sensorValue = analogRead(A0) - 512;
  Serial.println(sensorValue);
  delay(1);
}
