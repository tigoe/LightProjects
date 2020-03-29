void setup() {

}

void loop() {
  tone(A0, 440);
  tone(A0, 350);

delay(1000);
noTone(A0);
delay(1000);
}
