/*
  APA102x loadTest with Adafruit_DotStar library

  This sketch  loops through all the pixels of a APA102x-compatible device
  one pixel at a time, adding color cumulatively, in the order:
  blue, green, red

  If your power supply is not sufficient to control all your LEDs,
  then this sketch will fail when the power supply reaches its maximum current.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's DotStar library: https://github.com/adafruit/Adafruit_DotStar

  created 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_DotStar.h>

const int pixelCount = 12;    // number of pixels
// if you're using software SPI, you need these pins:
const int dataPin = 8;        // data pin
const int clockPin = 9;       // clock pin

// set up strip:
//Adafruit_DotStar strip(pixelCount, dataPin, clockPin, DOTSTAR_BRG);
// if you're using hardware SPI, use the built-in SPI CLK and MOSI pins
// for clock and data:
Adafruit_DotStar strip(pixelCount, DOTSTAR_BRG);

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
