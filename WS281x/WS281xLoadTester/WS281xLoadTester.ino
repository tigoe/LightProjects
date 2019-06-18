/*
  WS281x loadTest with Adafruit_DotStar library

  This sketch  loops through all the pixels of a WS281x-compatible device
  one pixel at a time, adding color cumulatively, in the order:
  blue, green, red

  If your power supply is not sufficient to control all your LEDs,
  then this sketch will fail when the power supply reaches its maximum current.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's DotStar library: https://github.com/adafruit/Adafruit_DotStar

  created 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 4;  // control pin
const int pixelCount = 12;    // number of pixels


// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB+ NEO_KHZ800);

unsigned long color = 0xFF;   // start with blue

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);   // set the color for this pixel
    strip.show();    // refresh the strip
    delay(500);
  }

  if (color >= 0xFFFFFF) {   // if the color is greater than white (0xFF000000)
    color = 0xFF;              // then set it back to blue
  } else {
    color = (color << 8) + 0xFF; // add the next color at full
  }
}
