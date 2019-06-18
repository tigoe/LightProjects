/*
  WWA WS281x strip using ColorConverter

  This sketch uses the Adafruit NeoPixel library to drive a strip of
  WS281x warm white-cool white-amber (WWA) LEDs
  and the ColorConverter library to do warm-to-cool conversion.
  A potentimeter on A0 fades the strip from warm to cool.

  The arrangement of LEDs was discovered through trial and error.

  created 11 Feb 2019
  modified 17 June 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

const int neoPixelPin = 4;
const int pixelCount = 20;    // number of pixels

// set up strip. Note BRG setting. You might have to experiment
// to determine which way your LEDs are wired:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_BRG + NEO_KHZ800);
ColorConverter converter;

int s = 100;        // saturation
int i = 100;        // intensity

void setup() {
  Serial.begin(9600);
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
}

void loop() {
  int sensorReading = analogRead(A0);
  // map the result from 0 - 240
  // (not 360, because you want warm to cool, not
  // warm to cool and back:
  int mix = map(sensorReading, 0, 1023, 0, 240);
  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGB(mix, s, i);

  // change the ColorConverter labels to something you can use:
  int amber = color.red;
  int warm = color.green;
  int cool = color.blue;

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set the color for this pixel:
    strip.setPixelColor(pixel, amber, warm, cool);
  }
  // update the strip
  strip.show();
}
