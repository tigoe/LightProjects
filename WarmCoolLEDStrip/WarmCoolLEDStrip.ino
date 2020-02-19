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
int intensity = 0;      // LED intensity (0-255)

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read potentiometer, use it to set ratio of warm to cool:
  int potReading = analogRead(A0) / 4;
  int warm = intensity * (255 - potReading) / 255;
  int cool = intensity * (potReading / 255);
  // PWM the LED channels:
  analogWrite(4, warm);
  analogWrite(5, cool);
}
