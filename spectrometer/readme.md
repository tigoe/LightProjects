# Building Your Own Spectrometer

It's common in any lighting practice to need a spectrometer to measure the properties of light. Good spectrometers are expensive, though. Fortunately, there are some affordable spectral sensors that you can connect to a microcontroller to make your own spectrometer. This tutorial shows you how to connect the AMS [AS7341 11-channel spectral sensor](https://ams.com/as7341#tab/documents) to an [Arduino Nano 33 IoT](https://store-usa.arduino.cc/products/arduino-nano-33-iot). The AS7341 will work with any microcontroller that has an I2C interface, but the Nano 33 IoT has a Bluetooth and WiFi radio, making it easy to connect to via multiple means.

Below, you'll see how to get output from your spectrometer via multiple means: 
* UART serial to [p5.js](https://p5js.org)
* WiFi and MQTT to an MQTT broker, with a p5.js client

## The AS7341 Sensor

The AS7341 is an 11-channel spectral sensor that can detect light levels in multiple frequencies from around 400nm to 900nm. It has 8 that sense light in visible spectrum; one channel in the near infrared spectrum; one clear channel without a filter; and one channel that detects 50Hz-60Hz light flicker. 

Here is the [product page](https://ams.com/as7341) and the [datasheet](https://ams.com/documents/20143/36005/AS7341_DS000504_3-00.pdf/5eca1f59-46e2-6fc5-daf5-d71ad90c9b2b).

The frequencies it can detect are as follows:
* F1: 415nm <span style="color:white; background-color:#7600ed;">(violet)</span>
* F2: 445nm <span style="color:white; background-color:#0028ff;">(blue)</span>
* F3: 480nm <span style="color:white; background-color:#00d5ff;">(light blue)</span>
* F4: 515nm <span style="color:white; background-color:#1fff00;">(green)</span>
* F5: 555nm <span style="color:white; background-color:#b3ff00;">(yellow green)</span>
* F6: 590nm <span style="color:white; background-color:#ffdf00;">(yellow)</span>
* F7: 630nm <span style="color:white; background-color:#ff4f00;">(orange)</span>
* F8: 680nm <span style="color:white; background-color:#ff0000;">(red)</span>
* NIR: 910nm (near IR)
* Clear: non filtered
* FD: (Flicker Detection) non filtered

The following companies make breakout boards for it: 
* [Sparkfun](https://www.sparkfun.com/products/17719)
* [Adafruit](https://www.adafruit.com/product/4698)
* [DFRobot](https://www.dfrobot.com/product-2132.html)

The following companies make Arduino libraries for it:
* [Adafruit](https://github.com/adafruit/Adafruit_AS7341)
* [Sparkfun](https://github.com/sparkfun/SparkFun_AS7341X_Arduino_Library)

## The Circuit

The circuit for all of the applications below is the same. These diagrams show [Adafruit's AS7341 breakout boards](https://www.adafruit.com/product/4698), but any breakout board for the AS7341 will work. The AS7341 connects to the Nano 33 IoT via I2C. For the Arduino Uno or Nano boards, the I2C pins are pins A4 (SDA) and A5 (SCL). The sensor's SDA pin connects to the Arduino's SDA pin and the SCL pin connects to the Arduino's SCL pin. The sensor's voltage and ground pins connect to the microcontroller's power and ground pins as well. This is the same connection for almost any I2C sensor. Figures 1 and 2 below shows the connections. 

![Breadboard view of an AMS AS7341 sensor connected to a Nano 33 IoT](img/AS7341_fritzing_bb.png)

_Figure 1. Breadboard view of an AMS AS7341 sensor connected to a Nano 33 IoT. The Nano's SDA pin (physical pin 8) is connected to the sensor's SDA pin (physical pin 4) and the Nano's SCL pin (physical pin 9) is connected to the sensor's SCL (physical pin 3). The sensor's Vdd (physical pin 1) is connected to the Nano's 3V3 pin (physical pin 2) and the sensor's ground (physical pin 2) is connected to the Nano's ground (physical pin 14)._

![Schematic view of an AMS AS7341 sensor connected to a Nano 33 IoT](img/AS7341_fritzing_schem.png)

_Figure 1. Schematic view of an AMS AS7341 sensor connected to a Nano 33 IoT. The connections are as described above: SDA to SDA, SCL to SCL, voltage and ground to voltage and ground, respectively._

## Serial to p5.js

The simplest way to see the readings from the AS7341 is by printing the values out from the Nano's serial port (UART). This example does just that. It prints the sensor readings, converts them to basic counts, applies a daylight correction, and sends the values out to the serial monitor. For an explanation of the process, see the [Spectral Sensor
Calibration Methods](https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740)  application note, section 2.1 - 2.5. 

__TO DO:__ The correction math in these examples is crude, and simply applies the numbers in Fig. 10 of the application note with no further math. I haven't yet verified its accuracy. 

* [raw code](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341)
* [p5.js application in gitHub pages](https://tigoe.github.io/LightProjects/spectrometers/AS7341/spectrograph_chartjs_serial/index.html)

## WiFi and MQTT Clients



