# Light Parts

This is a list of parts used in building this repository, and a few more that might be useful.

## Programmable LEDs (WS2182b, SK6812, APA102C or equivalent):
* [WS281x modules](http://www.ledlightinghut.com/8-sk6812-rgbw-5050-digital-led-ring.html) - There are many variations on this. It's useful to keep one of the small 4-8 LED modules handy for testing code, even if you're planning to use longer strips in your actual project.
* [ SK6812 RGBW Digital Addressable LED Strip, 60LEDs/m, 5VDC](http://www.ledlightinghut.com/sk6812-4-in-1-rgbw-led-strip.html) - These typically come in 60 LEDs/m or 144 LEDs/m
* [Programmable LED strips, WWA, 60 LEDs (or greater) per meter, 5V](http://www.ledlightinghut.com/144led-sk6812-wwa-led-strip.html). These are WWA versions of the WS281x LEDs. The same code will control them, but you get tunable white instead of colors.

## LED Strips (not individually addressable):
* [Tunable White LED Strip Light/Tape Light - 12V - 350 Lumens/ft ](https://www.superbrightleds.com/moreinfo/flexible-led-strip-lights/3528-tunable-white-led-strip-lighttape-light-24v-ip20-350-lumensft/5497/12019/). Strips like these come in variable brightness as well as length and density.
* [RGBW LED Strip Lights, 245 Lumens/ft](https://www.superbrightleds.com/moreinfo/flexible-led-strip-lights-color-changing/outdoor-rgbw-led-strip-lights-weatherproof-12v-led-tape-light-w-white-and-multicolor-leds-245-lumensft/1711/)
* [Low Profile Aluminum LED Strip Channel - Surface Mount LED Extrusion - KLUS MICRO-ALU Series](https://www.superbrightleds.com/moreinfo/aluminum-channels/klus-b1888_k7-micro-alu-series-surface-mount-black-anodized-aluminum-led-profile-housing/2039/4738/) - These are used to mount LED strips. They come with a diffuser lens which fits in the channel.
* [Klus 0945 - HR-ALU series LED Profile Heavy Duty Frosted Lens](https://www.superbrightleds.com/moreinfo/housing-accessories/klus-0945--hr-alu-series-led-profile-heavy-duty-frosted-lens/961/2363/) - These are a heavier diffuser than the one that comes with the strips above.

## DC power supplies:
* [Wall-Mounted AC Adapter - 12 VDC Power Supply - 12-36W](https://www.superbrightleds.com/moreinfo/power-supplies-accessories/12v-dc-cps-series-power-supply/4273/9569/) - Many standard 12V DC supplies will do the job. This will pair with a 2.1mm I.D./5.5mm O.D. DC jack.
* [Mean Well LED Switching Power Supply, AP Series 12-35W Single Output LED Power Supply, 12V DC](https://www.superbrightleds.com/moreinfo/power-supplies/12vdc-mean-well-led-power-supply--apv-series/1748/5179/) - I usually get these because they're a constant current supply. They are good for powering LED strips. Get them with power cord or get a power cord that will work from a local electrical supplier.

## Connectors:
* [DC Barrel Jack Adapter, Male](https://www.superbrightleds.com/moreinfo/bar-strip-connectors/cps-f2st-female-standard-barrel-connector-to-screw-terminal-adapter/856/) - These are for making connections to 2.1mm I.D./5.5mm OD D.C. power supplies like the one above.
* [DC Barrel Jack Adapter, Female](https://www.superbrightleds.com/moreinfo/bar-strip-connectors/cps-f2st-female-standard-barrel-connector-to-screw-terminal-adapter/856/) - To connect to the item above

## LED standard-base 12V bulbs (G6.35 2-pin base):
* [Bi-Pin Socket for GU5.3/G4/GX5.3/GY6.35/GZ4 Base Bulbs ](https://www.superbrightleds.com/moreinfo/household-bulb-sockets-adapters/mr16-socket-mr11-bi-pin-socket-for-gu53g4gx53gy635gz4-base-bulbs/499/2027/) - This socket works with the lamps below, and many others. 
* [GY6.35 LED Landscape Light Bulb - 40 Watt Equivalent - Bi-Pin LED Bulb - 450 Lumens ](https://www.superbrightleds.com/moreinfo/landscape-bulbs/gy635-led-landscape-light-bulb-40-watt-equivalent-bi-pin-led-bulb-450-lumens/4539/10110/)
* [GY6.35 LED Landscape Light Bulb - 40 Watt Equivalent - Bi-Pin LED Bulb - 275 Lumens ](https://www.superbrightleds.com/moreinfo/landscape-bulbs/gy635-led-landscape-light-bulb-40-watt-equivalent-bi-pin-led-bulb-275-lumens/4541/10111/)

## Components
**Transistors**. For most 12-24V sources, you'll need transistors to control them from a microcontroller. The following transistors have been tested with the examples here:
* [IRLB8721](https://www.digikey.com/products/en?keywords=IRLB8721PBF) - A MOSFET that operates well on 3.3V. Here's its [data sheet](https://www.infineon.com/dgdl/irlb8721pbf.pdf?fileId=5546d462533600a40153566056732591). It can switch a load up to 30V and theoretically 60A with proper head dissipation. 
* [FQP30N06L](https://www.digikey.com/products/en?keywords=FQP30N06L) - Another 3.3V-tolerant MOSFET. Here's its [datasheet](https://cdn.sparkfun.com/datasheets/Components/General/FQP30N06L.pdf). This one can switch 60V and 30A.  
* [TIP120](https://www.digikey.com/product-detail/en/stmicroelectronics/TIP120/497-2539-5-ND/603564) This Darlington transistor, well-known in Arduino circles for switching motors, will also work for switching LED sources. Here's its [data sheet](https://www.st.com/content/ccc/resource/technical/document/datasheet/f9/ed/f5/44/26/b9/43/a4/CD00000911.pdf/files/CD00000911.pdf/jcr:content/translations/en.CD00000911.pdf). It requires a base resistor of about 1 kilohm for the examples used here.

**Tangible controls**. These are included for example purposes only. You can use whatever input you want for your LED projects.
* [pushbuttons](https://www.digikey.com/products/en?keywords=PRT-14460) - I like these ones because they fit on a breadboard well but are big enough to push in a satisfying way. 
* [rotary encoders](https://www.digikey.com/product-detail/en/bourns-inc/PEC12R-4225F-S0024/PEC12R-4225F-S0024-ND/4499648) - These are breadboard-friendly as well, if you bend the side tabs up.
* [potentiometers](https://www.digikey.com/product-detail/en/tt-electronics-bi/P160KN2-4QC20B10K/987-1310-ND/2408887) - These are breadboard-friendly as well.

## Microcontrollers 
I tend to use the MKR series Arduinos a lot:
* [MKR Zero](https://store.arduino.cc/usa/arduino-mkrzero)
* [MKR 1010 WiFi/BLE](https://store.arduino.cc/usa/mkr-wifi-1010)
* [MKR 1000 WiFi](https://store.arduino.cc/usa/arduino-mkr1000)
* The [Nano 33 IoT](https://store.arduino.cc/usa/nano-33-iot) works well for these examples also