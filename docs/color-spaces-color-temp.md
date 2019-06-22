# Color Spaces and Color Temperature

In order to understand how light changes, it's helpful to know a few different ways of describing those changes, and to understand how different qualities of light are created. 

## Color Models

There are a number of different components to color and light. You can talk about component colors, either additive or subtractive; you can talk about hue, saturation, and lightness, or value. It's important to talk about how you're creating color and light, however, if you want to control it well.  Lighting designers tend to talk in terms of additive primary colors -- red, green, and blue -- because color of light is additive. In contrast, color reflected by a surface pigment is subrtactive, so in that context, we tend to talk in terms of subtractive primaries: red, yellow, and blue.

Our mental model of how we light changes generally aren't phrased in terms of primary colors, however. We might talk about how it dims, or how the colors become paler as it gets brighter, or we might talk about how a light gets warmer as it fades out. We might talk about how the sky shifts from a pale blue to brilliant oranges and reds as the sun sets, as in this quote:

<blockquote>"Soon it got dusk, a grapy dusk, a purple dusk over tangerine groves and long melon fields; the sun the color of pressed grapes, slashed with burgundy red, the fields the color of love and Spanish mysteries."

-Jack Kerouac, "On the Road"</blockquote>

These are terms that use a different color model than the primary-color models. Now we're talking about hue, saturation, and lightness, and color temperature.

White light is composed of multiple wavelengths of light, and when describing it, lighting designers and engineers speak about the color temperature of the light, referring to its relative warmth or coolness. Warmer light contains more longer wavelengths, toward the red end of the spectrum, while cooler light contains more shorter wavelengths, toward the blue end of the spectrum.

When you're fading light, then, you need to consider which color model will make for the best fade. Do you want the hue to stay consistent as the light dims? Do you want the light to get cooler or warmer as it changes? Or do you have another fade pattern in mind?

## Computational Color Descriptions
Multi-channel LED control systems like the ones described here describe the relative intensity of each color channel as separate numeric values, or they describe all the colors in one composite number. For example, if you're using RGB LEDs and assigning an 8-bit resolution to each channel, then you'll need three 8-bit numbers to describe what your light source is doing. For example, you might describe red light as 0xFF0000, meaning that the red channel is at 255 (in hexadecimal 0xFF) while the green and blue channels are both at 00. Teal might be described as 0x0088FF (no red, about half green, full blue). 

Now imagine you're writing a program to fade your LEDs. What would the  fade from the following code would look like?

````
 // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set the color for each pixel:
    strip.setPixelColor(pixel, color);   
  }
  // refresh the strip:
  strip.show();
  // decrement the color:
  color--;
  // if it reaches 0, set it back to 0xFFFFFF:
  if (color == 0) color = 0xFFFFFF;
````
 [Try it](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xRGBFade). It's probably not what you expect.

 You could rearrange things to that you're fading all three colors at the same time:

 ````// loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    // set the color for each pixel:
    strip.setPixelColor(pixel, color, color, color);
  }
  // refresh the strip:
  strip.show();
  // decrement the color:
  color--;
  // if it reaches 0, set it back to 0xFFFFFF:
  if (color == 0) color = 0xFF;
  // this fade moves faster, so slow it down so you can see it:
  delay(10);
 ````

This works pretty well for white, but when you try it for an arbitrary set of three colors, it doesn't work too well. If you want to change the  brightness without changing the color, you need a different color model.

### Hue, Saturation, and Brightness

This is where a [Hue, Saturation, Brightness (or Lightness)](https://programmingdesignsystems.com/color/color-models-and-color-spaces/index.html#color-models-and-color-spaces-JDQ1fRD) model becomes useful. In the HSL model, hue is positioned on a color cylinder, with red at the top (0 degrees), green at 120 degrees, and blue at 240 degrees. Saturation is the radius of the cylinder, and lightness is the depth of the cylinder. Here's an [interactive visualization](https://programmingdesignsystems.com/color/color-models-and-color-spaces/index.html#hsl-cylinder) from Rune Madsen's book _Programming Design Systems_. The advantage of this model is that it allows you to change lightness or saturation without changing the hue. It becomes much simpler to create a[ flickering candle](https://github.com/tigoe/LightProjects/tree/master/APA102x/APA102xCandle) that changes from red to yellow through orange, for example. 

## References

There are many different color models, and which one you use depends on the context in which you're talking about color. For a great discussion of [color theory](https://programmingdesignsystems.com/color/a-short-history-of-color-theory/index.html) and [color models and spaces](https://programmingdesignsystems.com/color/color-models-and-color-spaces/index.html), see Rune Madsen's online book _[Programming Design Systems](https://programmingdesignsystems.com)_. 

For a partial glossary ofthe physical properties of light, see the [lighting terminology page](https://itp.nyu.edu/classes/light/lighting-terminology/) from my class on light and interactivity.

For a great set of principles of light to consider in lighting design -— illuminance, luminance, color and temperature, height, density, and direction and distribution —- see _[Architectural Lighting: Designing With Light And Space (Architecture Briefs))](https://books.google.com/books/about/Architectural_Lighting.html?id=3QJlJPIX8-sC)_ by Herve Descottes with Ceclia E. Ramos.
