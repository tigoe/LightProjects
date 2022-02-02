# WS281x Addressable LEDs and the Adafruit_NeoPixel Library

The WS281x and SK281x LEDs can be controlled with Adafruit's [NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) in a variety of ways. The library supports RGB and RGBW LEDs, and it can set colors using RGB or RGBW values, or HSV values. The HSV converter does not use the white pixel if it's present, however. 

Following are some good practices for using the NeoPixel library successfully. Think of this as a quickstart guide. Adafruit's [NeoPixel UberGuide](https://learn.adafruit.com/adafruit-neopixel-uberguide) covers the best practices of NeoPixels in much more depth. 

## Initialization

There are a few properties you should know to  initialize a WS281x/SK281x module with the NeoPixel library. Here's the top of nearly every NeoPixel sketch:

````arduino
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);

````

The things you'll change are the pixel that you're controlling the LEDs from (`neoPixelPin`, above); the number of pixels you're controlling (`pixelCount`, above); and the arrangement of pixels (`NEO_GRB`, above). The arrangement of pixels depends on which LEDs you are using. Some WS218x-derivative LEDs arrange the order of LEDs in a different order than the Green-Red-Blue order suggested here. If you have an RGBW module, the last property usually the white LED.

To test the order, [here's a sketch](WS281x/WS281xColorOrderTester/WS281xColorOrderTester.ino) that will turn on blue, then green, then red, then white for all the LEDs.

