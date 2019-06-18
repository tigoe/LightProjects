/*
  APA102x HSV Test with Adafruit_DotStar library

  This sketch tests the HSV functionality of the Adafruit_DotStar library

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
unsigned int hue = 0;
int sat = 255;
int intensity = 255;

void setup() {
  Serial.begin(9600);
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  // increment hue and rollover at 65535:
  hue++;
  hue %= 65536;
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // get RGB from HSV:
    unsigned long color = strip.ColorHSV(hue, sat, intensity);
    // do a gamma correction:
    unsigned long correctedColor = strip.gamma32(color);
    strip.setPixelColor(pixel, correctedColor);   // set the color for this pixel
    Serial.println(hue);
  }
  strip.show();    // refresh the strip
}
