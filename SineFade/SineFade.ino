int x = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  x++;
  x = x % 360;
  float intensity = (sin(x * PI / 180) + 1) * 127;

  Serial.print(x);
  Serial.print(",");
  Serial.println(intensity);

  analogWrite(5, intensity);
  delay(30);
}
