/*
  APA102x candle using Adafruit_DotStar library

  This sketch creates a candle effect by randomizing
  hue, sat, and intensity

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
Adafruit_DotStar candle(pixelCount, DOTSTAR_BGR);
unsigned int hue = 3000;
int sat = 127;
int intensity = 127;

void setup() {
  candle.begin();    // initialize pixel strip
  candle.clear();    // turn all LEDs off
  candle.show();     // update strip
}

void loop() {
  for (int p = 0; p < pixelCount; p++) {
    // change hue -1 to 2 points:
    int hueChange = random(4) - 1;

    hue += hueChange;
    // constrain to red to orange:
    hue = constrain(hue, 0, 6000);

    // change saturation -1 to 2 points:
    int satChange = random(4) - 1;
    sat += satChange;
    sat = constrain(sat, 0, 255);

    // change intensity -1 to 1 points:
    int intensityChange = random(3) - 1;
    intensity += intensityChange;
    intensity = constrain(intensity, 0, 255);

    // get RGB from HSV:
    unsigned long color = candle.ColorHSV(hue, sat, intensity);
    // do a gamma correction:
    unsigned long correctedColor = candle.gamma32(color);
    candle.setPixelColor(p, correctedColor);;
  }

  candle.show();
  delay(30);
}
