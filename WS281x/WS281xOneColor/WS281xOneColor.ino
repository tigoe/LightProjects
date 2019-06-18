/*
  WS281x Test with Adafruit_NeoPixel library

  This sketch  sets the pixels to one color. Change pixelCount 
  to the number of LEDs in your string.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 4;  // control pin
const int pixelCount = 20;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, 0x423512); // set the color for this pixel
  }

  strip.show();    // refresh the strip
}
