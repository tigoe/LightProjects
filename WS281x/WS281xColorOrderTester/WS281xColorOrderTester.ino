/*
  WS281x Test with Adafruit_NeoPixel library

  This sketch  turns on all the LEDs first blue, then green, then red, then white.

  Change pixelCount to the number of LEDs in your string.

  Note: if you don't plan to use an RGBW string, change NEO_GRBW to NEO_GRB
  and change "  if (color < 0xFF000000) {" to "  if (color < 0xFF0000) {"

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 31 Jan 2017
  modified 27 May 2023
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 8;   // number of pixels
int color = 0xFF;           // blue

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
  Serial.begin(9600);
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);   // set the color for this pixel
    strip.show();    // refresh the strip
  }
  delay(1000);

  // shift color by 8 bits each time through the loop:
  if (color < 0xFF000000) {
    color = color << 8;     // shift to the next color
  } else {
    color = 0xFF;           // when you've gone through all the colors, shift to the first.
  }
}
