
#include <RTCZero.h>
#include <ArduinoLowPower.h>
const int cradleHookPin = 9;
RTCZero rtc;
int sleepMinutes = -1;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode (cradleHookPin, INPUT_PULLUP);

  LowPower.attachInterruptWakeup(digitalPinToInterrupt(cradleHookPin), wakePhone, FALLING);
  rtc.begin();

  rtc.setTime(0, 59, 50);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.println(rtc.getSeconds());

  if (rtc.getMinutes() == sleepMinutes) {
    sleepPhone();
  }

  if (digitalRead(cradleHookPin) == HIGH) {
    sleepMinutes = (rtc.getMinutes() + 3) % 60;
    Serial.println("sleep at: " + String(sleepMinutes));
  }

}

void sleepPhone() {
  Serial.println("Sleeping phone");
  digitalWrite(LED_BUILTIN, LOW);
  LowPower.sleep();
}

void wakePhone() {
  Serial.println("waking phone");
  digitalWrite(LED_BUILTIN, HIGH);
  sleepMinutes = -1;
}
