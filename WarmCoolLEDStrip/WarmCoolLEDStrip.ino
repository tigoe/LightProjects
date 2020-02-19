/*
  Warm-Cool lamp using 12V LED strips
    
  Reads a potentiometer on pin A1, uses it to set a ratio of warm to cool
  controls two LED channels on pins 4 and 5

  Circuit:   
        potentiometer on pin A1
        12V LEDs on pins 4 and 5

  created 19 Jan 2019
  modified 16 Feb 2020
  by Tom Igoe
*/
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read potentiometer, use it to set ratio of warm to cool:
  int potReading = analogRead(A0) / 4;
  int warm =  255 - potReading;
  int cool =  potReading;

  // PWM the LED channels:
  analogWrite(5, cool);
  analogWrite(6, warm);
}
