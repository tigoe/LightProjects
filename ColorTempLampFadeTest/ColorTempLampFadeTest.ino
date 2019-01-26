#include <Wire.h>
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 60;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);
unsigned long color = 0xFF;   // start with blue
int times = 0;

void setup(void) {
  Serial.begin(9600);
  delay(3000);
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop(void) {
  int sensorVal = analogRead(A0);
   byte warm =  sensorVal /4;
   byte cool = sensorVal /4;
   byte amber = sensorVal /4;
  for (int p = 0; p < pixelCount; p++) {
    strip.setPixelColor(p, color);// set the color for this pixel
  }
  Serial.println(color, HEX);
  color = color << 8;         // shift the FF (255) to the next color
  if (color > 0xFF0000) {   // if the color is greater than white (0xFF000000)
    color = 0xFF;             // then set it back to blue
  }
  strip.show();
  delay(5000);
}
