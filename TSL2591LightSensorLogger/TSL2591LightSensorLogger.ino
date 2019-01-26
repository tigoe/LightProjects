/* 
  TSL2591  Light Sensor example

   Modified and simplified from the Adafruit example

   created 31 Jan 2017
   by Tom Igoe
*/

#include <Wire.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591();

void setup() {
  Serial.begin(9600);
  if (tsl.begin()) {
    // gain can be low (1x), med(25x), high(428x):
    tsl.setGain(TSL2591_GAIN_MED);      
    
    // integration time can be 100, 200, 300, 400, 500, 600ms.
    // longer integration gathers more light:
    tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  } else {
    Serial.println("No sensor found. check your wiring.");
    while (true);     // stay here and do nothing
  }
Serial.println("Time\tLux:");
}

void loop() {
  unsigned long luminosity = tsl.getFullLuminosity();
  unsigned int ir = luminosity >> 16;           // IR is the bottom two bytes
  unsigned int full = luminosity & 0xFFFF;      // full is the top two bytes
  unsigned int visible = full - ir;             // visible is full - IR
  unsigned long lux = tsl.calculateLux(full, ir);// calculate lux from luminosity values
 
  Serial.println(lux);
}
