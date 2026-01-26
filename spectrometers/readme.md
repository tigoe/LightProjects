# Building Your Own Spectrometer

It's common in any lighting practice to need a light meter or spectrometer to measure the properties of light. Good spectrometers are expensive, though. For an example, see this page on the [Sekonic C-800-U](../light-rendering-indices.md) or the [Sekonic C-7000](https://sekonic.com/sekonic-spectrometer-c-7000/). Both are good meters, but not cheap.  

Fortunately, there are some affordable spectral sensors that you can connect to a microcontroller to make your own spectrometer. This tutorial offers some  [considerations on construction of a DIY spectrometer](spectrometer-construction.md), and shows how to connect one sensor, the AMS [AS7341 11-channel spectral sensor](https://ams.com/as7341#tab/documents) to an [Arduino Nano 33 IoT](https://store-usa.arduino.cc/products/arduino-nano-33-iot). The AS7341 will work with any microcontroller that has an I2C interface, but the Nano 33 IoT has a Bluetooth and WiFi radio, making it easy to connect to via multiple means.

In the [AS7341 examples](AS7341), you'll see how to get output from your spectrometer via multiple means: 
* UART serial to [p5.js](https://p5js.org)
* WiFi and MQTT to an MQTT broker, with a p5.js client
* Bluetooth LE to a Chrome browser, with a p5.js client

## Spectral Sensors to Consider

* AMS [AS7341 11-channel spectral sensor](https://ams.com/as7341#tab/documents). Used in many of the examples here. Unfortunately, no longer sold for current development, but breakout boards are still available. 
  * [Examples](AS7341/readme.md)
* AMS [AS7343 14-channel spectral sensor](https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-as7343-spectral-sensor) Unfortunately, no longer sold for current development, but breakout boards are still available. 
* [AMS' full line of spectral sensors](https://ams-osram.com/search?productSearch=true&filter_products=ambient-light-color-spectral-proximity-sensors&filter_param_function_String_sortable=spectral+sensing&filter_lifecycle_key_String_sortable=10-full-production%2C93-not-for-new-design%2C95-orders-still-accepted)
* [OPT4048 High-speed high-precision tristimulus XYZ color sensor]((https://www.ti.com/product/OPT4048)) from Texas Instruments. Gives CIE XYZ values, but only has three spectral bands. 
  * [Sparkfun breakout boards and hookup guide](https://docs.sparkfun.com/SparkFun_Tristimulus_Color_Sensor-OPT4048/introduction/)
  * [Basic example](OPT4048/OPT4048_test/) based on Sparkfun library
* AMS [TCS3448 14-channel spectral color sensor](https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-tcs3448-spectral-color-sensor)

## DIY Sensor Device Consruction

Some [notes](spectrometer-construction.md) on building your own spectrometer's housing. 