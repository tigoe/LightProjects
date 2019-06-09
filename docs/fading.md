# Fading Light

Light doesn't change in a linear fashion, and the human eye doesn't respond to light in a linear fashion. Light, like all forms of radiant energy, decreases with the square of the distance from the source. Furthermore, the human eye responds to light in a logarithmic fashion. In 1931, the International Commission on Illumination (CIE) released a study of *psychometric colorimetry*, or the measurement of how we perceived changes in light. The landmark study resulted in the [CIE 1931 standard colorimetric system](http://www.cie.co.at/eilv/150), a standardized way of describing perceived color. As part of that, they defined a [formula for lightness](http://www.photonstophotos.net/GeneralTopics/Exposure/Psychometric_Lightness_and_Gamma.htm) as we perceive it. In the CIE 1931 formula, lightness varies linearly from 0 to 8 percent, and on a  1/3 power function from 8 percent to 100 percent. 

Add to this the fact that different diffusers and reflectors affect the perceived perceived lightness of a source, and you'll see that lightness is a complex matter. As a result, it's helpful to have a few non-linear formulas for fading light when you're controlling light from a computer. Depending on the effect you're looking for, you might want a light source to start fading slowly and then speed up, or slow down at the end of its fade. 

## References
* https://programmingdesignsystems.com/color/perceptually-uniform-color-spaces/index.html#perceptually-uniform-color-spaces-e7zMSWy
* http://www.huevaluechroma.com/013.php
* http://www.cie.co.at/eilv/150
* http://www.photonstophotos.net/GeneralTopics/Exposure/Psychometric_Lightness_and_Gamma.htm