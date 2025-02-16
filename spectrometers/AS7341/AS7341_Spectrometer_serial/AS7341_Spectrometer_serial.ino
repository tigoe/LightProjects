/*
  AS7341 sensor readings.  Reads and prints the results
  as a CSV string. This version uses a non-blocking
  approach, using the startReading() and checkReadingProgress()
  functions. It's about 2 seconds between readings.
  Details for this sensor can be found on the AMS product page here:
  https://ams-osram.com/products/sensor-solutions/ambient-light-color-spectral-proximity-sensors/ams-as7341-11-channel-spectral-color-sensor
  In particular, the product data sheet, user guide, and 
  Application Note AS7341 AN000633, "Spectral Sensor Calibration Methods"
  are of most use. 

  Library:
  http://librarymanager/All#Adafruit_AS7341

  created 18 Jun 2021
  modified 16 Feb 2025
  by Tom Igoe
*/

#include <Adafruit_AS7341.h>
// instance of the sensor library:

Adafruit_AS7341 as7341;
// array to hold the raw readings:
uint16_t readings[12];
float channels[12];
// header string for CSV:
String headers = "415nm,445nm,480nm,515nm,555nm,590nm,630nm,680nm,Clear,NIR";
// correction factors:  these corrections are from Application Note AS7341 AN000633, "Spectral Sensor Calibration Methods"
// fig. 10. These are for channels F1 through F8.
// TODO: This math needs to be corrected.
// values 4 and 5 are 0 because they are not used (see datasheet)
float corrections[] = { 3.20, 3.00, 2.07, 1.30, 0.0, 0.0, 1.07, 0.93, 0.78, 0.71 };

void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);

  if (!as7341.begin()) {
    Serial.println("Sensor not found, check wiring");
    while (true)
      ;
  }

  // set integration time:
  as7341.setATIME(100);
  as7341.setASTEP(999);
  as7341.setGain(AS7341_GAIN_256X);
  // print column headers:
  Serial.println("setup");
  Serial.println(headers);
  // start a new reading cycle:
  as7341.startReading();
}

void loop() {
  if (!as7341.checkReadingProgress()) return;
  
  // if the current reading is complete:
    if(readSensor()) {
    // print the results:
    for (int r = 0; r < 12; r++) {
      // skip readings 4 and 5 as they are repeats:
      if (r == 4 || r == 5) continue;
      Serial.print(channels[r]);
      if (r < 11) Serial.print(",");
    }
    Serial.println();
    // start a new reading cycle:
    as7341.startReading();
  }
}

bool readSensor() {
  // take the current reading, do corrections,
  // and put the result into readingString:
  // get the readings:
  if (!as7341.readAllChannels(readings)) return false;
  // there are 12 elements in the readAllChannels array,
  // but elements 4 and 5 are not used. So channel number is different
  // than array index:
  int channelNum = 0;
  // loop over the readings and put them in the channels array:
  for (int r = 0; r < 12; r++) {
    // skip readings 4 and 5 as they are repeats:
    if (r == 4 || r == 5) continue;
    channels[r] = as7341.toBasicCounts(readings[r]);
    if (r < 10) {
      channels[r] = channels[r] * corrections[r];
    }
  }
  return true;
}
