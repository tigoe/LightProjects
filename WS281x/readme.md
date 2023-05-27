# WS281x Addressable LEDs and the Adafruit_NeoPixel Library

The [WS281x and SK681x LEDs](../addressable-leds.md#worldsemi-addressable-leds) can be controlled with Adafruit's [NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) in a variety of ways. The library supports RGB and RGBW LEDs, and it can set colors using RGB or RGBW values, or HSV values. The HSV converter does not use the white pixel if it's present, however. 

Following are some good practices for using the NeoPixel library successfully. Think of this as a quickstart guide. Adafruit's [NeoPixel UberGuide](https://learn.adafruit.com/adafruit-neopixel-uberguide) covers the best practices of NeoPixels in much more depth. 

## Initialization

There are a few properties you should know to  initialize a WS281x/SK681x module with the NeoPixel library. Here's the top of nearly every NeoPixel sketch:

````arduino
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);

````

Most examples you'll find will refer to the variable holding  the library instance as `strip`, but you can call the variable whatever you want. 

The things you'll change are the pixel that you're controlling the LEDs from (`neoPixelPin`, above); the number of pixels you're controlling (`pixelCount`, above); and the arrangement of pixels (`NEO_GRB`, above). The arrangement of pixels depends on which LEDs you are using. Some WS218x-derivative LEDs arrange the order of LEDs in a different order than the Green-Red-Blue order suggested here. If you have an RGBW module, the last property usually the white LED.

To test the order, [here's a sketch](WS281xColorOrderTester/) that will turn on blue, then green, then red, then white for all the LEDs. If the colors don't appear in that order you can change the initializer. Possible values are all permutations of RGB or RGBW. For example, GRB, GBR, RGB, GRBW, etc. Re-ordering the colors will affect which channels are controlled by which color values. Try different orders in the sketch above to see how it works. 

## Testing Your Modules

Before you do anything else you should test your modules to make sure that each pixel and each color channel on each pixel works. 

You can test each pixel with [this test]({{codeurl}}/WS281xTester/). It will turn on each pixel one at a time and one color at a time. 

You can test all the pixels of a given channel at once with [this test]({{codeurl}}/WS281xColorOrderTester/). It will turn on all the pixels of a given channel at a time. 

You can test a specific color with [this test]({{codeurl}}/WS281xOneColor/). It will listen for an incoming hexadecimaly serial string and set all the pixels with the value of that string. For example, sending `0,255,0,0` will set the pixels to red, `0,0,255,0` will set it to green, etc. 

## Setting Color Values

In the sketch above, all the colors are represented in variable, `color`. This allows you to use one hexadecimal number to set R, G, B, and W if you have it, at once, similar to HTML colors. For example:

````
0xFF      - blue
0xFF00    - green
0xFF0000  - red
0xFF00000 - white (if the white channel is present)
0xFFFFFF  - white (if the white channel is not present)
````

To set the color, you use `setPixelColor` like so:

````arduino
    strip.setPixelColor(pixel, color);
````
You can also represent color as four variables, like so:

````arduino
    strip.setPixelColor(pixel, red, green, blue);
    // or 
    strip.setPixelColor(pixel, red, green, blue, white);  // if there is a white channel
````
Which you choose depends on what you're trying to do with your colors. Sometimes the single variable is easier, sometimes three or four variables is easier. The latter makes isolation of the R, G, B, or W channels easier. The former makes setting complex colors other than red, green blue, or white easier. 

## Setting and Clearing the Module

The only time the LEDs in a module change are when you call one of two functions: `clear()` or `show()`. `clear()` will turn off all the LEDs at once. `show()` will update all the LEDs with whatever values you set them to using `setPixelColor()`. In the [Color order tester sketch](WS281xColorOrderTester/), All the module's colors are set in a `for` loop before the `show()` command is called. However, you can also test one pixel and one color at a time by calling `show()` whenever you call getPixelColor. Here's [another tester](WS281xTester/) that tests each pixel one at a time in each color. Both this and the earlier test are good to run with each new module, to make sure all the pixels work.

## Voltage and Current

As the Adafruit NeoPixel Uberguide explains, addressable LEDs take a lot of power: up to 20 milliamps per channel per pixel at 5V. That means that a 60-pixel RGBW strip takes 20mA * 4 channels * 60 pixels. That's 4800 mA, or 4.8 Amps when all the pixels are on. That's a lot!  More than you can run from a microcontroller running from your USB port, that's for sure. 

Here's a [sketch that turns on one color per pixel at a time](WS281xLoadTester/), adding colors and pixels one at a time as it goes. You can use this to test the current load of a strip. You should see the current go up by the amperage of each channel for each pixel per second. 

You can run a small number of WS281x pixels from 3.3V boards like the Nano 33 IoT. For small modules like the NeoPixel jewels (7 pixels), they work fine from the power from a Nano 33. However, for modules of more than a dozen or so pixels, you should power them from 5V, and if possible, use a 3.3V-to-5V level-shifter for the control pin.

## RGB vs HSV

Controlling addressable LEDs with RGB is fine as long as you only need to control one primary color at a time. However, sometimes you want to do more complex things, like fade from red to orange, or green to teal to blue. In these cases, it's easier if you can use the Hue, Saturation, and Value (HSV) color space. 

To get an RGB color from hue, saturation, and value, you use the `ColorHSV()` function like so:

````arduino
   unsigned long color = strip.ColorHSV(hue, sat, intensity);
````

This results in a single color value with the correct red, green, and blue values in it. You might also want to correct the gamma values before you set the colors like so:

````arduino
   unsigned long correctedColor = strip.gamma32(color);
    strip.setPixelColor(pixel, color);   // set the color for this pixel
````

Gamma correction will adjust the color intensities on a power law curve rather than a linear one, color-correcting the red, green, and blue to more appropriately match the hue, saturation, and value that you want. [This sketch](WS281xTester/) demonstrates the use of HSV colors. Here's [another sketch](WS281HSVColorDemo/) that prints out the colors as you move through the HSV color wheel. Here's a [variation on the previous sketch](WS281HSVColorDemoSerial/) that allows you to control the hue cycling with input from the Serial Monitor. 


