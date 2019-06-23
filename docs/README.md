# Light Projects

A collection of lighting projects controlled by microcontrollers.

## Low-voltage DC Lamps 

Low-voltage [LED Lamps](led-lamps.md) are easy to control from a microcontroller when you know what properties you need to be aware of. In fact, there are so many low-voltage sources on the market now that you can manage most indoor lighting needs without needing to control high-voltage AC sources. 

## LED Strip Control
Single-channel and multi-channel [LED strips](led-strips.md) are very popular these days, and pretty easy to control with a microcontroller and a few transistors.  You can find LED strips in a variety of combinations:
* Red/Green/Blue (RGB)
* Red/Green/Blue/White (RGBW). These come in cool white, warm white, and natural white variations.
* Tunable White. These come in a variety of styles: Warm/cool white, Warm/cool/natural white, and White/white/amber.

## Addressable LED Control
There are a number of [addressable LED components](addressable-leds.md) on the market now, and they come in a variety of form factors. Covered here are a couple of the most popular, the WS28xx/SK68xx LEDs, and the APA102C LEDs. Adafruit calls their products that use these LEDs NeoPixels and DotStars, respectively. SparkFun calls their APA102C line Lumenati. Whatever you call them, they're relatively easy to use, and there are many good tools for controlling them.  Addressable strips can be found in the many of the same variations as non-addressable strips, including RGB, RGBW, RGBAW, and WWA.

### Examples:
* [APA102xTester](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xTester)
* [APA102xLoadTester](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xLoadTester)
* [APA102xRGBFade](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xRGBFade)
* [APA102xRGBFade002](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xRGBFade002)
* [APA102x_SerialToColorConverter](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102x_SerialToColorConverter)
* [APA102x_HSV_Tester](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102x_HSV_Tester)
* [APA102xCandle](https://github.com/tigoe/LightProjects/tree/master/Candles/APA102xCandle)
* [WS281xTester](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281xTester)
* [WS281xLoadTester](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281xLoadTester)
* [WS281xOneColor](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281xOneColor)
* [WS281x_HSV_Tester](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281x_HSV_Tester)
* [WS281x_Mixer](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281x_Mixer)
* [WS281x_AdvancedMixer](https://github.com/tigoe/LightProjects/tree/master/WS281x/WS281x_AdvancedMixer)

## Fading
The relationship between perceived brightness and the power controlling a light source is non-linear. In otherwise, if you fade an LED source in a straight line, it won't appear to you to be fading evenly in time. Depending on the effect you're looking for, you might want a light source to start fading slowly and then speed up, or slow down at the end of its fade. It helps to [have methods for a few different fade curves](fading.md) available. 

### Examples
* [SimpleFade](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/SimpleFade)
* [SineFade](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/SineFade)
* [XSquaredFade](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/XSquaredFade)
* [ExponentialFade](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/ExponentialFade)
* [CIE1931Fade](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/CIE1931Fade)
* [CIE1931FadeWithKalmanInput](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/CIE1931FadeWithKalmanInput)
* [CIE1931FadeWithInput](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/CIE1931FadeWithInput)
* [SimpleFadeWithInput](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/SimpleFadeWithInput)
* [SineFadeWithInput](https://github.com/tigoe/LightProjects/tree/master/FadeCurves/SineFadeWithInput)

## Color and Color Temperature Fading
 
 Whether you're working with tunable white lighting sources or color sources, it's useful to understand a few things about
[Color Spaces and Color Temperature](color-spaces-color-temp.md) in order to get the most our of your sources.

### Examples
* [ColorTempFade](https://github.com/tigoe/LightProjects/tree/master/ColorTempFade/)
* [APA102xRGBFade](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xRGBFade)
* [APA102xRGBFade002](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xRGBFade002)

## Recommended Parts

This [list pf parts](inventory.md) is not comprehensive, it's just a list of some of the parts I've used in building this repository.



