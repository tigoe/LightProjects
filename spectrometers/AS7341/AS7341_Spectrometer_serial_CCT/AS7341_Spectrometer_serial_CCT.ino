#include <BasicLinearAlgebra.h>
#include <Adafruit_AS7341.h>

// use the BasicLinearAlgebra namespace.
//  You might need to add BLA::  before all the Matrix declarations
using namespace BLA;

// instance of the sensor library:
Adafruit_AS7341 as7341;


// XYZ correction matrix from the spreadsheet at
// https://ams.com/documents/20143/36005/AS7341_AD000198_3-00.xlsx
// (tab: used correction values, O6:X8):
Matrix<3, 10> corrMatrix = { 0.39814, 1.29540, 0.36956, 0.10902, 0.71942, 1.78180, 1.10110, -0.03991, -0.27597, -0.02347,
                             0.01396, 0.16748, 0.23538, 1.42750, 1.88670, 1.14200, 0.46497, -0.02702, -0.24468, -0.01993,
                             1.95010, 6.45490, 2.78010, 0.18501, 0.15325, 0.09539, 0.10563, 0.08866, -0.61140, -0.00938 };


// offsets and correction factors, from the spreadsheet:
// (tab: used Correction Values, O15:X15)):
Matrix<10> offsets = { 0.00197, 0.00725, 0.00319, 0.00131, 0.00147, 0.00186, 0.00176, 0.00522, 0.00300, 0.00100 };
// (tab: used Correction Values, O26:X26)):
Matrix<10> correctionFactors = { 1.02811, 1.03149, 1.03142, 1.03125, 1.03390, 1.03445, 1.03508, 1.03359, 1.23384, 1.26942 };

// sensor readings:
Matrix<10> readings;
// corrected sensor values:
Matrix<10> correctedValues;
// normalized sensor values:
Matrix<10> normalizedValues;

float lux;
float CCT;
float x;
float y;
float z;

void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);
  // check for the sensor:
  while (!as7341.begin()) {
    Serial.println("Sensor not found, check wiring");
    delay(1000);
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
  if (as7341.checkReadingProgress()) {
    readSensor();
    calculateLightValues();

    for (int r = 0; r < 10; r++) {
      // print to 5 decimal places:
      // TODO: should I print normalized values?
      Serial.print(correctedValues(r), 5);
      if (r < 9) Serial.print(",");
    }
    Serial.println();

    Serial.print("Lux: ");
    Serial.println(lux, 5);

    Serial.print("   CCT: ");
    Serial.print(CCT, 5);

    Serial.print("   x: ");
    Serial.print(x, 5);
    Serial.print("  y: ");
    Serial.print(y, 5);
    Serial.print("  z: ");
    Serial.println(z, 5);
    Serial.println();

    // start a new reading cycle:
    as7341.startReading();
  }
}

void readSensor() {
  // check to see if readings are done
  //  checkReadingProgress() is non-blocking:
  uint16_t data[12];
  // if there is a reading, do corrections and parse it
  // into readingString:

  // get the readings:
  as7341.getAllChannels(data);
  int c = 0;
  // loop over the readings and print them out:
  for (int r = 0; r < 12; r++) {
    // skip 4 and 5 as they are repeats:
    if (r == 4 || r == 5) continue;
    readings(c) = as7341.toBasicCounts(data[r]);
    // increment channel counter:
    c++;
  }
}

void calculateLightValues() {
  // XYZ readings:
  Matrix<3> XYZ;
  // zero out the corrected and normalized matrices:
  correctedValues.Fill(0);
  normalizedValues.Fill(0);
  // max reading for the normalization:
  float maxReading = 0.0;

  // loop over the readings and print them out:
  for (int r = 0; r < 10; r++) {
    correctedValues(r) = correctionFactors(r) * (readings(r) - offsets(r));
    // thisReading = thisReading * corrections[r];
    maxReading = max(correctedValues(r), maxReading);
  }

  // not really using this anywhere:
  for (int r = 0; r < 10; r++) {
    normalizedValues(r) = correctedValues(r) / maxReading;
  }
  // from spreadsheet, tab: demonstration calculations, J10-12:
  XYZ = corrMatrix * correctedValues;

  // from spreadsheet, tab: demonstration calculations, J14-16:
  float XYZSum = XYZ(0) + XYZ(1) + XYZ(2);
  x = XYZ(0) / XYZSum;
  y = XYZ(1) / XYZSum;
  z = XYZ(2) / XYZSum;

  // not clear on the origin of the constants below:
  // from spreadsheet, tab: demonstration calculations, J17:
  lux = XYZ(1) * 683;
  // from spreadsheet, tab: demonstration calculations, J22:
  CCT = 437 * pow(((x - 0.332) / (0.1858 - y)), 3) + 3601 * pow(((x - 0.332) / (0.1858 - y)), 2) + 6861 * ((x - 0.332) / (0.1858 - y)) + 5517;
}