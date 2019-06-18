/*
  APA102x Test with Adafruit_DotStar library

  This sketch  loops through all the pixels of a APA102x-compatible device
  one pixel at a time, and one color at a time, in the order:
  blue, green, red

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's DotStar library: https://github.com/adafruit/Adafruit_DotStar

  created 31 Jan 2017
  modified 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_DotStar.h>

const int pixelCount = 4;    // number of pixels
// you need these only if you're using sotware SPI:
const int dataPin = 8;       // data pin
const int clockPin = 9;      // clock pin

// set up strip:
//Adafruit_DotStar strip(pixelCount, dataPin, clockPin, DOTSTAR_BRG);
Adafruit_DotStar strip(pixelCount, DOTSTAR_BGR);
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
    if (pixel > 0) {
      strip.setPixelColor(pixel - 1, 0); // turn off the last pixel
    }
    strip.show();    // refresh the strip
    delay(500);
  }

  if (color >= 0xFF0000) {   // if the color is greater than red (0xFF0000)
    color = 0xFF;              // then set it back to blue
  } else {
    color = color << 8;        // shift the FF (255) to the next color
  }
  strip.clear();               // clear the strip at the end of a color
}
