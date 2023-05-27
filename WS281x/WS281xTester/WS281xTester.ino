/*
  WS281x Test with Adafruit_NeoPixel library

  This sketch  loops through all the pixels of a WS281x-compatible device
  one pixel at a time, and one color at a time, in the order:
  blue, green, red, white

  Change pixelCount to the number of LEDs in your string.

  Note: if you don't plan to use an RGBW string, change NEO_GRBW to NEO_GRB
  and the line "if (color > 0xFF000000) {" to "if (color > 0xFF0000) {"

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 31 Jan 2017
  modified 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW+NEO_KHZ800);
unsigned long color = 0xFF;   // start with blue

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
  delay(2000);      // delay 2 seconds before starting
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);   // set the color for this pixel
    if (pixel > 0) {
      strip.setPixelColor(pixel - 1, 1); // turn off the last pixel
    }
    strip.show();    // refresh the strip
    delay(250);      // 1/4 sec delay after each pixel
  }

  if (color >= 0xFF000000) {   // if the color is greater than white (0xFF000000)
    color = 0xFF;              // then set it back to blue
  } else {
    color = color << 8;        // shift the FF (255) to the next color
  }
  strip.clear();               // clear the strip at the end of a color
}
