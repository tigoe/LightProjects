/*
  APA102x RGB fade with Adafruit_DotStar library

  This sketch  fades a string of APA102x LEDs by simply counting
  down from 0xFFFFFF (white) to 0x000000 (off). It fades 
  all three colors at the same time.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's DotStar library: https://github.com/adafruit/Adafruit_DotStar

  created 17 Jun 2019
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
unsigned long color = 0xFF;
void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set the color for each pixel:
    strip.setPixelColor(pixel, color, color, color);
  }
  // refresh the strip:
  strip.show();
  // decrement the color:
  color--;
  // if it reaches 0, set it back to 0xFFFFFF:
  if (color == 0) color = 0xFF;
  delay(10);
}
