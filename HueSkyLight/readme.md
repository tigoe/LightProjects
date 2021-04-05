# Hue color temperature control example for ArduinoHttpClient library

This example reads a TCS34725 light sensor to get the color temperature. It then uses ArduinoHttpClient library to control Philips Hue. For more on Hue developer API see http://developer.meethue.com

To control a light, the Hue expects a HTTP PUT request to:
http://hue.hub.address/api/hueUserName/lights/lightNumber/state

The body of the PUT request looks like this:
````
  {"on": true, 
  "bri": brightness,  // ranges from 0 - 254
  "ct": value,        // where value ranges from 153 - 500
  "transitiontime": fadeTime  //in increments of 100ms
  }
````  
  
ct is in the mired scale, which is 1000000/degrees Kelvin

This example  uses the Arduino_JSON library to assemble the
PUT request and the body of the request.

Works with MKR1010, MKR1000, Nano 33 IoT.
Uses the following libraries:
http://librarymanager/All#WiFi101  // use this for MKR1000
http://librarymanager/All#WiFiNINA  // use this for MKR1010, Nano 33 IoT
http://librarymanager/All#ArduinoHttpClient
http://librarymanager/All#Arduino_JSON
http://librarymanager/All#Adafruit_TCS347525

note: WiFi SSID and password are stored in arduino_secrets.h file.
If it is not present, add a new tab, call it "arduino_secrets.h"
and add the following defines, and change to your own values:
````
#define SECRET_SSID "ssid"
#define SECRET_PASS "password"
````

created 5 april 2021
by Tom Igoe (tigoe)