/*
  WS281x RGB fade with Adafruit_NepPixel library

  This sketch  fades a string of WS281x LEDs by simply counting
  down from 0xFFFFFF (white) to 0x000000 (off). It makes no consideration
  for the different colors.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 17 Jun 2019
  modified 27 May 2023
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

unsigned long color = 0xFFFFFF;
void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set the color for each pixel:
    strip.setPixelColor(pixel, color);
  }
  // refresh the strip:
  strip.show();
  // decrement the color:
  color--;
  // if it reaches 0, set it back to 0xFFFFFF:
  if (color == 0) color = 0xFFFFFF;
}
