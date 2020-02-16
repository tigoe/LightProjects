
const int rotaryPin = 2;
const int endRotaryPin = 3;
const int cradleHookPin = 9;
const int debounceDelay = 12;

volatile bool dialing = false;
volatile int dialCount = 0;
volatile bool hungUp = true;

void setup() {
  Serial.begin(9600);
  pinMode (rotaryPin, INPUT_PULLUP);
  pinMode (endRotaryPin, INPUT_PULLUP);
  pinMode (cradleHookPin, INPUT_PULLUP);
  hungUp = digitalRead(cradleHookPin);
  attachInterrupt(digitalPinToInterrupt(rotaryPin), count, FALLING);
  attachInterrupt(digitalPinToInterrupt(endRotaryPin), dial,  CHANGE);
}

void loop() {
  if (!dialing) {
    hungUp = digitalRead(cradleHookPin);
  }
  if (hungUp) {
    Serial.println("hung up");
    delay(500);
  }
  if (millis() % 1000 < 1 && !dialing) {
    Serial.print("Dial count: ");
    Serial.println(dialCount);
    delay(1);
  }
}



void dial() {
  delay(debounceDelay);
  dialing = !digitalRead(endRotaryPin);
  if (dialing) {
    dialCount = 0;
  }
}

void count() {
  delay(debounceDelay);
  if (digitalRead(rotaryPin) == LOW) {
    dialCount++;
  }
}
