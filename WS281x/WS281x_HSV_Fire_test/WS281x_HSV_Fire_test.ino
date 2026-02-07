/*
  APA102x HSV fire Test with Adafruit_NeoPixel library

  This sketch tests the HSV functionality of the Adafruit_NeoPixel library
  Sets each pixel at a random color in the red-to-orange range (0-2000)
  and gives it a random increment. Then sets them all oscillating.

  Change pixelCount to the number of LEDs in your string.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 31 Jan 2017
  modified 6 Nov 2022
  by Tom Igoe
*/
// If you're using a SAMD-based board like the Nano 33 IoT, then the ZeroDMA library
// runs much faster than the standard NeoPixel library, but can only use certain pins
// On Arduino NANO 33 IoT it can use pins 4, 6, 7, A2, A3, 11.
// #include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel_ZeroDMA.h>

const int neoPixelPin = 4;  // control pin
const int pixelCount = 60;  // number of pixels

// set up strip:
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel_ZeroDMA strip = Adafruit_NeoPixel_ZeroDMA(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

// hues and changes for each pixel:
unsigned int hue[pixelCount];
int hueDelta[pixelCount];
int sat = 255;
int intensity = 255;
int maxHue = 2000;

void setup() {
  Serial.begin(9600);
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn all LEDs off
  strip.show();   // refresh strip
  // set the random seed:
  randomSeed(analogRead((A0)));
  //set up the arrays:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set a hue for each pixel, in the desired range:
    hue[maxHue] = random(maxHue);
    // set a random increment for the pixel:
    hueDelta[pixel] = random(-1, 2) * 3;  // -3 to 3
    // don't let it be zero:
    if (hueDelta[pixel] == 0) hueDelta[pixel] = -1;
  }
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // increment hue:
    hue[pixel] += hueDelta[pixel];
    // reverse direction at the extremes:
    if (hue[pixel] <= 0 || hue[pixel] >= maxHue) {
      hueDelta[pixel] = -hueDelta[pixel];
    }

    // get RGB from HSV:
    unsigned long color = strip.ColorHSV(hue[pixel], sat, intensity);
    // do a gamma correction:
    unsigned long correctedColor = strip.gamma32(color);
    strip.setPixelColor(pixel, color);  // set the color for this pixel
  }
  strip.show();  // refresh the strip
}