/*
  APA102x HSV Test with Adafruit_NeoPixel library

  This sketch tests the HSV functionality of the Adafruit_NeoPixel library

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 31 Jan 2017
  modified 17 Jun 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 4;  // control pin
const int pixelCount = 20;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);

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
  hue %= 65535;
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // get RGB from HSV:
    unsigned long color = strip.ColorHSV(hue, sat, intensity);
    // do a gamma correction:
    unsigned long correctedColor = strip.gamma32(color);
    strip.setPixelColor(pixel, color);   // set the color for this pixel
    Serial.println(hue);
  }
  strip.show();    // refresh the strip
}
