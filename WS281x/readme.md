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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

````

Most examples you'll find will refer to the variable holding  the library instance as `strip`, but you can call the variable whatever you want. 

The things you'll change are the pixel that you're controlling the LEDs from (`neoPixelPin`, above); the number of pixels you're controlling (`pixelCount`, above); and the arrangement of pixels (`NEO_GRBW`, above). The arrangement of pixels depends on which LEDs you are using. Some WS218x-derivative LEDs arrange the order of LEDs in a different order than the Green-Red-Blue order suggested here. The most popular  options:

* `NEO_GRBW` - four-channel module with RGB and white channel
* `NEO_GRB` - three-channel module with RGB only

Some third-party modules may have the channels out of order, so you can try `RGB`, `RGBW`, `BRG`, `BRGW`, etc. 

To test the order, [here's a sketch]({{site.codeurl}}/WS281x/WS281xColorOrderTester/) that will turn on blue, then green, then red, then white for all the LEDs. If the colors don't appear in that order you can change the initializer. Re-ordering the colors will affect which channels are controlled by which color values. Try different orders in the sketch above to see how it works. 

## Testing Your Modules

Before you do anything else you should test your modules to make sure that each pixel and each color channel on each pixel works. 

You can test each pixel with [this test]({{site.codeurl}}/WS281x/WS281xTester/). It will turn on each pixel one at a time and one color at a time. 

You can test all the pixels of a given channel at once with [this test]({{site.codeurl}}/WS281x/WS281xColorOrderTester/). It will turn on all the pixels of a given channel at a time. 

You can test a specific color with [this test]({{site.codeurl}}/WS281x/WS281xOneColor/). It will listen for an incoming hexadecimaly serial string and set all the pixels with the value of that string. For example, sending `0,255,0,0` will set the pixels to red, `0,0,255,0` will set it to green, etc. 

## Controlling your NeoPixel Module

There are a few commands that you'll use all the time in the NeoPixel library. At the beginning of your program, you always make a copy of the library in a variable:

````arduino
// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
````
The variable is often called `strip` because many NeoPixel modules are strips. But you can call it anything you want. Among other things, this variable stores the values for every pixel's red, green, blue, and white channels. (note: not all NeoPixel modules have a white channel).

Then you usually set up your NeoPixel module at the start of the program:
````arduino
 strip.begin();  // initialize pixel strip
 strip.clear();  // turn all LEDs off
 strip.show();   // refresh strip
 ````

 The latter commands, `.clear()` and `.show()`, get used in the main loop to change the module as well. `.clear()` turns off all the LEDs, and `.show()` updates all the LEDS to the values stored in the `strip` variable. 

 In the main loop, you'll often iterate over all the pixels with a `for` loop, and either set them all at once like so:
````arduino
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
      // make some change to a single pixel's color here,
      // then set the new color in the strip variable: 
      strip.setPixelColor(pixel, color);  
  }
  // when the loop is done, refresh the module:
  strip.show();
  ````
  
  Or you might change one pixel at a time, like so:
  ````arduino
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
      // make some change to a single pixel's color here,
      // then set the new color in the strip variable: 
      strip.setPixelColor(pixel, color);  
      // update the strip. Only the pixel
      // that you changed will look different:
      strip.show();
      // add a delay to allow for seeing each pixel change:
      delay(100);
  }

  ````

  There are endless variations you could use to change and animate your module. These two approaches are just a start.

## Setting Color Values

There are several ways you can set the color for a pixel. 

The following method sets the color for a single pixel, using four numbers, representing red, green, blue and white: 
````arduino
    strip.setPixelColor(pixel, red, green, blue);
    // or 
    strip.setPixelColor(pixel, red, green, blue, white);  // if there is a white channel
````

Alternately, you can the color for a single pixel using a single number, representing red, green, blue and white: 
````arduino
 strip.setPixelColor(pixel, color);
 ````

With this approach, color values are generally written in hexadecimal (base-16), with two characters representing each color. For example:

* 0xFF000000 is white, full intensity
* 0x7F000000 is white, full intensity
* 0x00FF0000 is red
* 0x0000FF00 is green
* 0x000000FF is blue

 And finally, you can set colors using hue, saturation, and intensity like so:
This generates a single color from hue (0-65535), sat (0-255), and intensity (0-255) values:
   unsigned long color = strip.ColorHSV(hue, sat, intensity);
This does a correction on the color to adjust the brightness: 
    unsigned long correctedColor = strip.gamma32(color);
Then this sets the color, just like above: 
    strip.setPixelColor(pixel, color);  // set the color for this pixel

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
Which you choose depends on what you're trying to do with your colors. Sometimes three or four variables is easier, as it makes isolation of the R, G, B, or W channels easier. The single value is more efficient code-wise, and it makes setting complex colors other than red, green blue, or white easier. 

## RGB vs HSV

Controlling addressable LEDs with RGB is fine as long as you only need to control one primary color at a time. However, sometimes you want to do more complex things, like fade from red to orange, or green to teal to blue. In these cases, it's easier if you can use the Hue, Saturation, and Value (HSV) color space. 

To get an RGB color from hue, saturation, and value, you use the `.colorHSV()` function like so:

````arduino
   unsigned long color = strip.ColorHSV(hue, sat, intensity);
````

This results in a single color value with the correct red, green, and blue values in it. You might also want to correct the gamma values before you set the colors like so:

````arduino
   unsigned long correctedColor = strip.gamma32(color);
    strip.setPixelColor(pixel, color);   // set the color for this pixel
````

Gamma correction will adjust the color intensities on a power law curve rather than a linear one, color-correcting the red, green, and blue to more appropriately match the hue, saturation, and value that you want. [This sketch]({{site.codeurl}}/WS281x/WS281x_HSV_Tester/) demonstrates the use of HSV colors. Here's [another sketch]({{site.codeurl}}/WS281x/WS281HSVColorDemo/) that prints out the colors as you move through the HSV color wheel. Here's a [variation on the previous sketch]({{site.codeurl}}/WS281x/WS281HSVColorDemoSerial/) that allows you to control the hue cycling with input from the Serial Monitor. 


## Setting and Clearing the Module

The only time the LEDs in a module change are when you call one of two functions: `clear()` or `show()`. `clear()` will turn off all the LEDs at once. `show()` will update all the LEDs with whatever values you set them to using `setPixelColor()`. In the [Color order tester sketch]({{site.codeurl}}/WS281x/WS281xColorOrderTester/), All the module's colors are set in a `for` loop before the `show()` command is called. However, you can also test one pixel and one color at a time by calling `show()` whenever you call getPixelColor. Here's [another tester]({{site.codeurl}}/WS281x/WS281xTester/) that tests each pixel one at a time in each color. Both this and the earlier test are good to run with each new module, to make sure all the pixels work.

## Voltage and Current

As the Adafruit NeoPixel Uberguide explains, addressable LEDs take a lot of power: up to 20 milliamps per channel per pixel at 5V. That means that a 60-pixel RGBW strip takes 20mA * 4 channels * 60 pixels. That's 4800 mA, or 4.8 Amps when all the pixels are on. That's a lot!  More than you can run from a microcontroller running from your USB port, that's for sure. 

Here's a [sketch that turns on one color per pixel at a time]({{site.codeurl}}/WS281x/WS281xLoadTester/), adding colors and pixels one at a time as it goes. You can use this to test the current load of a strip. You should see the current go up by the amperage of each channel for each pixel per second. 

You can run a small number of WS281x pixels from 3.3V boards like the Nano 33 IoT. For small modules like the NeoPixel jewels (7 pixels), they work fine from the power from a Nano 33. However, for modules of more than a dozen or so pixels, you should power them from 5V, and if possible, use a 3.3V-to-5V level-shifter for the control pin.
