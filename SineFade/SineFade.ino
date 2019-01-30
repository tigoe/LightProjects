/*
  Sine fade
  Takes a linear input from an analog sensor
  and produces a sinusoidal fade curve

  created 25 Jan 2019
  by Tom Igoe
*/
int intensity = 255;
int change = 1;
void setup() {
  Serial.begin(9600);
}

void loop() {
  // decrease or increase by 1 point each time
  // if at the bottom or top, change the direction:
  if (intensity <= 0 || intensity >= 255) {
    change = -change;
  }
  intensity += change;
  
  // get the sine of the intensity so you're not doing a linear fade:
  float result = sineFade(intensity, 0, 255);
  analogWrite(5, result);
  delay(5);
  Serial.println(result));
}

float sineFade(int inValue, int minValue, int maxValue) {
  // map input to a 0-179 range:
  float angle = map(inValue, minValue, maxValue, 0, 179);
  /* here's the explanation of the calulation:
    // convert to radians:
    float result = angle * PI/180;
    // now add PI/2 to offset by 90 degrees:
    result = result + PI/2;
    // get the sine of that:
    result = sin(result);
    // now you have -1 to 1. Add 1 to get 0 to 2:
    result = result + 1;
    // multiply to get 0-255:
    result = result * 127.5;
  */
  //here it all is in one line:
  float result = (sin((angle * PI / 180) + PI / 2) + 1) * 127.5;
  return result;
}
