/*
  WS281x candle using Adafruit_NeoPixel library

  This sketch creates a candle effect by randomizing
  hue, sat, and intensity for each pixel

  Note: in the Adafruit libraries, hue is a 16-bit number
  where red: 64530 - 1006
  yellow:  10902 - 10944
  etc.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 17 Jun 2019
  modified 4 July 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 4;  // control pin
const int pixelCount = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel candle = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

// arrays for each pixel's hue, sat, and intensity:
unsigned int hue[pixelCount];
int sat[pixelCount];
int intensity[pixelCount];

void setup() {
  // initialize the random number generator using a reading
  // from pin A6 (not connected to anything, so it will generate
  // a random number):
  randomSeed(analogRead(A6));
  
// loop over the pixel arrays:
  for (int p = 0; p < pixelCount; p++) {
    // generate a random initial value for each pixel's
    // hue, saturation and intensity:
    hue[p] = random(2000, 5000);      // red-orange to mid-orange
    sat[p] = random(192, 255);        // high end of saturation
    intensity[p] = random(127, 192);  // mid-high range of intensity
  }
  
  candle.begin();    // initialize pixel strip
  candle.clear();    // turn all LEDs off
  candle.show();     // update strip

}

void loop() {
  // loop over the pixels:
  for (int p = 0; p < pixelCount; p++) {
    // change hue -1 to 2 points:
    int hueChange = random(-1,2);
    hue[p] += hueChange;
    // constrain to red to orange:
    hue[p] = constrain(hue[p], 800, 8000);

    // change saturation -1 to 1 points:
    int satChange = random(-1,1);
    sat[p] += satChange;
    // constrain to the top end of the range:
    sat[p] = constrain(sat[p], 192, 255);

    // change intensity -2 to 3 points:
    int intensityChange = random(-2,3);
    intensity[p] += intensityChange;
    // constrain to 20-255
    intensity[p] = constrain(intensity[p], 20, 255);

    // get RGB from HSV:
    unsigned long color = candle.ColorHSV(hue[p], sat[p], intensity[p]);
    // do a gamma correction:
    unsigned long correctedColor = candle.gamma32(color);
    candle.setPixelColor(p, correctedColor);
  }
  candle.show();
  delay(5);
}
