# Controlling Low-Voltage DC Lamps

Low-vootage DC lamps are relatively easy to control from a microcontroller. The properties you need to consider are mostly the same ones to consider when [controlling LED strips](led-strips.md). Although it is possible to control AC dimmer circuits from a microcontroller, it's both more complex and less safe than working with low-voltage DC, and with the increasing amount of good DC lighting available, it's less necessary.

## Electrical and Physical Characteristics
Here's what to look for when shopping for lamps that you can control using a microcontroller.

**Voltage** - The most common voltages for DC lamps are +12V, and+24V. Even if you're using a 5V microcontroller like the Arduino Uno, you'll need a separate power source for your lamps and a transistor to control each lamp, because the current of each lamo will be greater than the microcontroller's GPIO pins can supply.

Be alert when shopping, as there are many high-voltage AC lamps on the market as well. These are not controllable using the methods described here. 

**Current** - The current on a given lamp will vary for each lamp. It should be listed on the site you purchase it from, but often you'll only be given the wattage. If so, remember that `wattage = voltage * current`, and you can work out the current. For example, this [12-24V landscape bulb](https://www.superbrightleds.com/moreinfo/landscape-bulbs/gy635-led-landscape-light-bulb-40-watt-equivalent-bi-pin-led-bulb-450-lumens/4539/10110/#tab/specifications) operates at 12V and consumes 5 watts. That means its current is about 0.4 amps.

You should leave some overhead in choosing a power supply; 20% is a good number. For example, if your lamp need 0.4 amps, then pick at least a 0.5 amp supply. 

**Equivalent Wattage** - don't be fooled by this number. It's a measure of the equivalent incandescent lamp you'd need, but the number has no bearing on your circuit. 


**Lamp Shape and Base** - different lamps come in different shapes and sizes, and have different bases. Always match your base to your lamp. There's a great guide to standard base shapes and their names in _Light Sources, Second Edition: Basics of Lighting Technologies and Applications_, page 47, by Spiros Kitsinelis. You can find a partial summary on my [lighting terminology page](https://itp.nyu.edu/classes/light/lighting-terminology/).

Bases you'll commonly find in DC lamps are the smaller screw bases, the bi-pin base, the twist-lock base, and the wedge base. Bulbs.com has an excellent [chart of bases](https://www.bulbs.com/learning/basechart.aspx). They also have a good [lamp size and shape chart](https://www.bulbs.com/learning/shapesandsizes.aspx). Generally the larger lamp sizes are limited to high voltage lamps, but you'll see candleabra lamps, MR-16 lamps like the JC bi-pin and the wedge frequently in LED form. 

<u style="color:green;">Size note:</u> The number in most lamp sizes refers to eighths of an inch. For example, a PAR64 lamp is a Parabolic Aluminizer Reflector (PAR), 64/8" (8 inches) in diameter. An MR-16 is a Metallic Reflector (MR), 16/8" (2 inches) in diameter. 

**Color Temperature** - Color temperature, measured in degrees Kelvin (°K), refers to the warmth or coolness of a light source. the higher the Kelvin temperature, the cooler the color hue. Here are the typical terms and their ranges indegrees Kelvin

* warm white: ~3000K
* cool white: ~5400K
* natural white: ~4000K
* amber: ~1800-2000K
* daylight: ~6500K and above

**Lumen output** - the brightness of any light source can be measured in *lumens*. For a good description of the main physical terms used to quantify lighting, see [this video from Philips](https://www.youtube.com/watch?v=9nPIzyV1mW0). For more on this, see  Kitsinelis, Spiros, _Light Sources, Second Edition: Basics of Lighting Technologies and Applications_ or Descottes, Hervé; Ramos, Cecilia E. (2013-07-02), _Architectural Lighting: Designing With Light And Space (Architecture Briefs)_ or my [lighting terminology page](https://itp.nyu.edu/classes/light/lighting-terminology/).

Here's an example of how to work with this information:

Illuminance, the density of light energy on a surface, decreases with the square of the distance from the source. Illuminance levels are usually given in lux, which is lumens/meters^2. So if you have [a source that's 450 lumens/m](https://www.superbrightleds.com/moreinfo/landscape-bulbs/gy635-led-landscape-light-bulb-40-watt-equivalent-bi-pin-led-bulb-450-lumens/4539/10110/#tab/specifications), and you expect it to light a wall that's 2 meters away from the source, then the illuminance on the wall would be 112.5 lux. Here are some suggested illuminance levels (from Descottes, Hervé; Ramos, Cecilia E. (2013-07-02). _Architectural Lighting: Designing With Light And Space (Architecture Briefs)_ ):

* Incidental use: 200 lx
* General work/office use: 300 lx
* Task lighting: 5-700 lx
* Specialized work: 1000-1500 lx
* Midday sun: 32K – 100K lx

## Transistors to Control LED Sources 
You can control low-voltage lamps with the same circuitry that you use to control non-addressable LED strips. Figure 1 below shows a typical MOSFET control circuit from a MKR Zero. 

![Figure 1. FQP30N06L MOSFET controlling a low-voltage LED lamp.](img/12VDC_LED_MOSFET_lamp_circuit_bb.png)

_Figure 1. MKR Zero connected to a FQP30N06L MOSFET for controlling a low-voltage LED lamp._ 

If you're holding the FQP30N06L MOSFET with the tab at the top and the bulk of the component facing you, then the pins, from left to right, are: gate, drain, source. The gate should be connected to the GPIO pin of the microcontroller that you plan to use to control the LEDs. The drain should be connected to the cathode of the LED source, and the source pin should be connected to ground.  For a full description of this circuit and a bipolar transistor variation on it, see the [Controlling LED Strips](led-strips.md#transistors-to-control-led-sources) page.
