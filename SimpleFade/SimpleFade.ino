int brightness = 0;
int change = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
}
void loop() {
  // add change to brightness, and constrain to 0-255:
  brightness = constrain(brightness + change, 0, 255);
  // if brightness is at either extreme, change the
  // direction of fading:
  if (brightness == 0 || brightness == 255) {
    change = -change;
  }
  // change the light:
  analogWrite(5, brightness);
  delay(10);
}
