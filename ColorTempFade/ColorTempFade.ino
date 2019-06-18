/*
  Warm to cool fader
  Fades two channels of LEDs, one warm white, one cool white
  Intensity fader fades the whole level down and up.
  
  Circuit:
        - 10-kilohm potentiometer on pin A0.
        - 10-kilohm potentiometer on pin A1
        - 12V LEDs on pins 4 and 5

  created 19 Jan 2019
  modified 17 June 2019
  by Tom Igoe
*/

void setup() {

}

void loop() {
  // read intensity potentiometer:
  int intensity = analogRead(A0) / 4;
  delay(1);
  // read color temp potentiometer, use it to set ratio of warm to cool:
  int colorTemp = analogRead(A1) / 4;
  // calculate warm and cool values:
  int warm = intensity * ((255 - colorTemp) / 255.0);
  int cool = intensity * (colorTemp / 255.0);
  // PWM the LED channels:
  analogWrite(4, warm);
  analogWrite(5, cool);
}
