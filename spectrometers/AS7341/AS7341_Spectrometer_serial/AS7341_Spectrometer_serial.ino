/*
  AS7341 sensor readings.  Reads and prints the results
  as a CSV string. This version uses a non-blocking
  approach, using the startReading() and checkReadingProgress()
  functions. It's about 2 seconds between readings.

  Library:
  http://librarymanager/All#Adafruit_AS7341


  created 18 Jun 2021
  modified 3 Oct 2021
  by Tom Igoe
*/

#include <Adafruit_AS7341.h>
// instance of the sensor library:

Adafruit_AS7341 as7341;
// array to hold the raw readings:
uint16_t readings[12];
// string to hold the corrected values as a CSV string:
String readingString = "";

void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);

  if (!as7341.begin()) {
    Serial.println("Sensor not found, check wiring");
    while (true);
  }

  // set integration time:
  as7341.setATIME(35);
  as7341.setASTEP(10000);
  as7341.setGain(AS7341_GAIN_256X);
  // start a new reading cycle:
  as7341.startReading();
}


void loop() {
// read the sensor, print if there are good values:
  if (readSensor()) {
    Serial.println(readingString);
  }
}

bool readSensor() {
  // clear the reading string:
  readingString = "";
  // check to see if readings are done
  //  checkReadingProgress() is non-blocking:

  float result = as7341.checkReadingProgress();
  // if there is a reading, do corrections and parse it
  // into readingString:
  if (result) {
    // correction factors:
    // TODO: This is crude, and could be better with matrix math.
    // from https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740
    // fig 10

    // Added in 0 for 4 and 5 since those channels are not used below.
    float corrections[] = {3.20, 3.00, 2.07, 1.30, 0, 0, 1.07, 0.93, 0.78, 0.71};

    // get the readings:
    as7341.getAllChannels(readings);
    // loop over the readings and print them out:
    for (int r = 0; r < 12; r++) {
      // skip 4 and 5 as they are repeats:
      if (r == 4 || r == 5) continue;
      float thisReading = as7341.toBasicCounts(readings[r]);
      // if this is one of the channels with a correction, apply it:
      if (r < 10) {
        thisReading = thisReading * corrections[r];
      }
      readingString += String(thisReading);
      if (r < 11) readingString += ",";
    }

    // start a new reading cycle:
    as7341.startReading();
  }
  return result;
}
