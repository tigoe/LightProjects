/*
  Squeeze bulb lamp

  Reads an MPX2012 sensor on pin A0, looks for peaks, and increments
  a variable called intensity with each peak.
  Reads a potentiometer on pin A1, uses it to set a ratio of warm to cool
  controls two LED channels on pins 4 and 5

  Circuit:
      * MPX2102 attached to an LM358 op amp attached to pin A0.
      * potentiometer on pin A1
      * 12V LEDs on pins 4 and 5
    See circuit drawing for details

  created 19 Jan 2019
  by Tom Igoe
*/
#include <SimpleKalmanFilter.h>

// first 2 numbers are the amount of variation,
// third is how fast the measurement moves. recommended: 0.01:
SimpleKalmanFilter filter(2, 2, 0.01);

int intensity = 0;      // LED intensity (0-255)
int lastEstimate = 0;   // last sensor estimate from the Kalman filter
int peakValue = 0;      // peak value
int threshold = 605;    // threshold for peak reading

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  // read the input:
  float sensorValue = analogRead(A0);
  // calculate the estimated value using the Kalman Filter:
  float estimate = filter.updateEstimate(sensorValue);
 
// look for a peak, above the threshold:
  if (estimate > peakValue && estimate > threshold) {
    peakValue = estimate;
  }
  if (estimate <= threshold) {
    if (peakValue > threshold) {
      // you have a peak value:
      Serial.print("peak: ");
      Serial.println(peakValue);
      // at each peak, increase the intensity by 10%
      intensity += 25;
      // keep it at or below 255:
      intensity = min(intensity, 255);
      // reset the peak variable:
      peakValue = 0;
    }
  }

  // every second, decrement the intensity by ~1%:
  if (millis() % 1000 < 2) {
    intensity -= 2;
    // keep it at or above 0:
    intensity = max(intensity, 0);
  }
  // read potentiometer, use it to set ratio of warm to cool:
  int potReading = analogRead(A1) / 4;
  int warm = intensity * (255 - potReading) / 255.0;
  int cool = intensity * (potReading / 255.0);
  // PWM the LED channels:
  analogWrite(4, warm);
  analogWrite(5, cool);
}
