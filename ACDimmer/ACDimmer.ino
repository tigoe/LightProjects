/*

  AC phase controlled dimming

  based on
  // https://playground.arduino.cc/Main/ACPhaseControl
  // http://alfadex.com/2014/02/dimming-230v-ac-with-arduino-2/

*/

const int triacPin = 4;
const int zeroCrossPin = 5;
const int timingFactor = 65;
const int firingTime = 12;
volatile int fadeLevel = 0;

void setup() {
  Serial.begin(9600);
  pinMode(triacPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(zeroCrossPin), zeroCross, RISING);
}

void loop() {
  int sensorReading = analogRead(A0);
  fadeLevel = sensorReading / 8;
  Serial.println(fadeLevel);
}

/*
  Generally,
  offTime = fadeLevel/fadeRange * (halfCycle - firingTime)
  For 60Hz,
  halfCycle = 1/120 sec = 8.33 ms = 8333 microseconds
  For the triac, firingTime = 10 microseconds, so:
  offTime = fadeLevel/128 * (8333 - 10)
  offTime = fadeLevel * (8323/128)
  offTime = fadeLevel * 65
  let's call 65 the timingFactor
  note that the light goes off above zero, and it's dependent on the light type
  e.g. incandescents fade differently than LEDs. 
  And this formula is for offTime, not onTime. 
  So to get the fade right, you need to work out the subset of the range, 
  e.g. 7 - 120, that's actually operable. TBD.
*/

void zeroCross() {
  // if the light should be off, don't fire the triac, just return:
  if (fadeLevel == 0) return;
  // if the triac should be on, delay for the offTime:
  int offTime = timingFactor * (128-fadeLevel);
  delayMicroseconds(offTime);
  // then fire the triac:
  digitalWrite(triacPin, HIGH);
  // then delay for the firingTime:
  delayMicroseconds(firingTime);
  // then turn the triac off:
  digitalWrite(triacPin, LOW);
}
