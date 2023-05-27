/*
  WS281x Test with Adafruit_NeoPixel library

  This sketch  sets the pixels to one color. Change pixelCount 
  to the number of LEDs in your string. It listens on the serial port 
  for a 4 comma-separated values:  white, red, green, blue, all 0-255.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 17 Jun 2019
  modified 27 May 2023
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 8;   // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn all LEDs off
  strip.show();   // refresh strip
}

void loop() {
  unsigned long color = 0;

// incoming serial string should be 4 comma-separated values: 
// white, red, green, blue. All 0-255:
  if (Serial.available()) {
    int white = Serial.parseInt();
    int red = Serial.parseInt();
    int green = Serial.parseInt();
    int blue = Serial.parseInt();
    // shift all the colors int one variable to set pixels:
    color = (white << 24) + (red << 16) + (green << 8) + blue;
    Serial.println(color, HEX);
  }

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);  // set the color for this pixel
  }

  strip.show();  // refresh the strip
}
