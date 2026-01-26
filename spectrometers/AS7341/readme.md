# The AS7341 Sensor
The AS7341 is an 11-channel spectral sensor that can detect light levels in multiple frequencies from around 400nm to 900nm. It has 8 that sense light in visible spectrum; one channel in the near infrared spectrum; one clear channel without a filter; and one channel that detects 50Hz-60Hz light flicker. 

Here is the [product page](https://ams.com/as7341) and the [datasheet and other docs](https://ams-osram.com/support/download-center?search=AS7341).

**Update 23 Jun 2025**: AMS lists the AS7341 as "Not for new design". While it's still possible to buy AS7341 breakout boards, it's also worth looking at some of their other models, like the [TCS3448](https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-tcs3448-spectral-color-sensor). 

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
* [Adafruit](https://www.adafruit.com/product/4698)
* [DFRobot](https://www.dfrobot.com/product-2132.html)

The following companies make Arduino libraries for it:
* [Adafruit](https://github.com/adafruit/Adafruit_AS7341)
* [Sparkfun](https://github.com/sparkfun/SparkFun_AS7341X_Arduino_Library)

## The Circuit

The circuit for all of the applications below is the same. These diagrams show [Adafruit's AS7341 breakout boards](https://www.adafruit.com/product/4698), but any breakout board for the AS7341 will work. The AS7341 connects to the Nano 33 IoT via I2C. For the Arduino Uno or Nano boards, the I2C pins are pins A4 (SDA) and A5 (SCL). The sensor's SDA pin connects to the Arduino's SDA pin and the SCL pin connects to the Arduino's SCL pin. The sensor's voltage and ground pins connect to the microcontroller's power and ground pins as well. This is the same connection for almost any I2C sensor. Figures 1 and 2 below shows the connections. 

![Breadboard view of an AMS AS7341 sensor connected to a Nano 33 IoT](../../img/AS7341_fritzing_bb.png)

_Figure 1. Breadboard view of an AMS AS7341 sensor connected to a Nano 33 IoT. The Nano's SDA pin (physical pin 8) is connected to the sensor's SDA pin (physical pin 4) and the Nano's SCL pin (physical pin 9) is connected to the sensor's SCL (physical pin 3). The sensor's Vdd (physical pin 1) is connected to the Nano's 3V3 pin (physical pin 2) and the sensor's ground (physical pin 2) is connected to the Nano's ground (physical pin 14). Image created using [Fritzing](https://fritzing.org)._

![Schematic view of an AMS AS7341 sensor connected to a Nano 33 IoT](../../img/AS7341_fritzing_schem.png)

_Figure 1. Schematic view of an AMS AS7341 sensor connected to a Nano 33 IoT. The connections are as described above: SDA to SDA, SCL to SCL, voltage and ground to voltage and ground, respectively. Image created using [Fritzing](https://fritzing.org)._

The [Sparkfun Qwiic Nano Shield](https://www.sparkfun.com/sparkfun-qwiic-shield-for-arduino-nano.html) is also a convenient way to connect many I2C sensor breakout boards to the Nano. 

## Serial to p5.js Client

The simplest way to see the readings from the AS7341 is by printing the values out from the Nano's serial port (UART). This example does just that. It prints the sensor readings, converts them to basic counts, applies a daylight correction, and sends the values out to the serial monitor. For an explanation of the process, see the [Spectral Sensor
Calibration Methods](https://ams-osram.com/support/download-center?search=spectral+calibration)  application note, section 2.1 - 2.5. 

__TO DO:__ The correction math in these examples is crude, and simply applies the numbers in Fig. 10 of the application note with no further math. I haven't yet verified its accuracy. 

This browser-based client reads and visualizes the data from serial port in this repository. It uses [p5.js](https://p5js.org), [chart.js](https://www.chartjs.org/) and the [p5.serialport library](https://github.com/p5-serial/p5.serialport) and [p5SerialControl](https://github.com/p5-serial/p5.serialcontrol/tags) app. Figure 3 shows a screenshot. This is similar to the appearance of the next few client apps below.

![screenshot of the serial-to-chartjs client](../../img/chartjs_spectrometer.png)
_Figure 3. Screenshot of the Serial client in a browser. Ten colored bands representing the sensor's frequencies are visible in a chart in the browser window._

* [raw JS code](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/spectrograph_chartjs_serial/)
* [Arduino Serial client](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/AS7341_Spectrometer_serial)
* [p5.js application in gitHub pages](https://tigoe.github.io/LightProjects/spectrometers/AS7341/spectrograph_chartjs_serial/index.html)

## WebSerial to p5.js Client

The WebSerial to p5.js Client uses the same chart.js display code as the previous example, but uses [webSerial](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API) instead of p5.serialport. It does not require the p5.serialcontrol app. It will work in the Chrome and Edge and Opera browsers, but not Safari or Firefox as of this writing. It's otherwise the same as the previous client, but useful to compare to see the difference between webSerial and p5.serialport.

* [raw JS code](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/spectrograph_chartjs_web-serial/)
* [Arduino Serial client](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/AS7341_Spectrometer_serial)
* [p5.js application in gitHub pages](https://tigoe.github.io/LightProjects/spectrometers/AS7341/spectrograph_chartjs_web-serial/index.html)

## WiFi and MQTT Client

If you've never connected to WiFi using the Nano 33 IoT, you should visit the [WiFiNINA reference](https://www.arduino.cc/en/Reference/WiFiNINA) and [this repository](https://tigoe.github.io/Wifi101_examples/). 

The Message Queueing Telemetry Transfer (MQTT) protocol was made for sensor monitoring over IP networks, so it's a good choice for a WiFi-based system for the spectrometer. MQTT is a __publish-and-subscribe__ or __PubSub__ model, in which clients connect to a broker application and subscribe to different topics. When they have new data, they publish to the topics. Topics can be broken up into subtopics when useful as well. There is very little overhead to the protocol; you just subscribe, then either publish or wait for messages. For more on MQTT, see [this repository](https://tigoe.github.io/mqtt-examples/), or [this definition from IBM](https://developer.ibm.com/technologies/messaging/articles/iot-mqtt-why-good-for-iot), who developed the protocol. 

There are several free online brokers that you can use. The most popular is called  [mosquitto](http://mosquitto.org/). You can run mosquitto on your own computer, or you can use [test.mosquitto.org](https://test.mosquitto.org/) as a test broker. The instructions and port numbers for the various ways of reaching it are listed on the site. [Shiftr.io](https://next.shiftr.io/try) is another MQTT test broker, with a graphic interface so you can see a graph of clients and topics. [mqtt.eclipse.org](https://mqtt.eclipse.org/) is another MQTT test broker with a fairly bare-bones set of documentation and no visualizer.

Once you have a broker set up, you enter your credentials, log in, subscribe, and start sending messages to publish. You can send messages in any format: plain text, CSV, JSON, or whatever suits your needs. 

The criteria you need to log into any broker are:
* The broker address
* The broker port (usually 1883 for unencrypted communication, 8883 for encrypted)
* Your client ID (you can make this up)
* The topic you want to subscribe to
* Some will expect a username and password as well.

Each broker will publish their details for login on their website. 

This MQTT example for the Nano 33 IoT publishes to shiftr.io using the topic _spectrometer_. It uses MQTTS, the encrypted version of the protocol, on port 8883. You can modify it to communicate with other brokers by changing the credentials. It uses the same sensor process as the example above: it prints the sensor readings, converts them to basic counts, applies a daylight correction, and sends the values out via MQTT. It prints them serially as well.

This browser-based client uses the [mqtt.js library](https://github.com/mqttjs/MQTT.js/) and [chart.js](https://www.chartjs.org/) like the serial client above. 

* [raw JS app code](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/spectrograph_chartjs_mqttjs/)
* [Arduino MQTT client](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/AS7341_Spectrometer_MQTT)
* [mqtt.js application in gitHub pages](https://tigoe.github.io/LightProjects/spectrometers/AS7341/spectrograph_chartjs_mqttjs/index.html)

## Bluetooth LE Client using p5ble.js

The Arduino Nano 33 IOT, MKR WiFi 1010, and Nano 33 BLE and BLE Sense boards can all send data via Bluetooth LE, using the [ArduinoBLE library](https://www.arduino.cc/en/Reference/ArduinoBLE). To complement this, [p5ble.js](https://itpnyu.github.io/p5ble-website/) is a great library for enabling BLE connections from the Chrome browser. The browser-based BLE client in this repo is based on the same chart.js code for visualizing the data as the previous two. It uses [p5.js](https://p5js.org), [chart.js](https://www.chartjs.org/) and the [p5.ble library](https://itpnyu.github.io/p5ble-website/). 

Unfortunately this sketch does not work on every browser, particularly on mobile platforms. However, it has been tested successfully on Chrome on MacOS and Windows; Chrome on Android, and [WebBLE](https://apps.apple.com/us/app/webble/id1193531073) on iOS. 

* [raw JS app code](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/spectrograph_chartjs_ble)
* [Arduino BLE client](https://github.com/tigoe/LightProjects/tree/main/spectrometers/AS7341/AS7341_Spectrometer_BLE)
* [p5.js application in gitHub pages](https://tigoe.github.io/LightProjects/spectrometers/AS7341/spectrograph_chartjs_ble/index.html)

[A few notes on melanopic EDI readings](../melanopic-edi-readings.md)