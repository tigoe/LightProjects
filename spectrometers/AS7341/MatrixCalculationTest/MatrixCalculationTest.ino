#include <BasicLinearAlgebra.h>
// use the BasicLinearAlgebra namespace.
//  You might need to add BLA::  before all the Matrix declarations
using namespace BLA;

// XYZ correction matrix from the spreadsheet:
Matrix<3, 10> corrMatrix = { 0.39814, 1.29540, 0.36956, 0.10902, 0.71942, 1.78180, 1.10110, -0.03991, -0.27597, -0.02347,
                             0.01396, 0.16748, 0.23538, 1.42750, 1.88670, 1.14200, 0.46497, -0.02702, -0.24468, -0.01993,
                             1.95010, 6.45490, 2.78010, 0.18501, 0.15325, 0.09539, 0.10563, 0.08866, -0.61140, -0.00938 };

// sensor readings:
Matrix<10> readings = {
  0.04411,
  0.11519,
  0.14046,
  0.14953,
  0.38247,
  0.41821,
  0.44645,
  0.11951,
  0.66134,
  0.07856
};
Matrix<10> corrections = { 3.20, 3.00, 2.07, 1.30, 1.07, 0.93, 0.78, 0.71, 0, 0 };

Matrix<10> offsets = {
  0.00197,
  0.00725,
  0.00319,
  0.00131,
  0.00147,
  0.00186,
  0.00176,
  0.00522,
  0.00300,
  0.00100,
};

Matrix<10> correctionFactors = { 1.02811, 1.03149, 1.03142, 1.03125, 1.03390, 1.03445, 1.03508, 1.03359, 1.23384, 1.26942 };


void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);
  calculate();
}


void loop() {
}

bool calculate() {
  // corrected sensor values:
  Matrix<10> correctedValues;
  // normalized sensor values:
  Matrix<10> normalizedValues;
  // XYZ readings:
  Matrix<3> XYZ;
  // zero out the corrected and normalized matrices:
  correctedValues.Fill(0);
  normalizedValues.Fill(0);
  // max reading for the normalization:
  float maxReading = 0.0;

  // loop over the readings and print them out:
  for (int r = 0; r < 10; r++) {
    // skip 4 and 5 as they are repeats:
    //if (r == 4 || r == 5) continue;

    // if this is one of the channels with a correction, apply it:
    //  if (r < 10) {
    correctedValues(r) = correctionFactors(r) * (readings(r) - offsets(r));
    // thisReading = thisReading * corrections[r];
    maxReading = max(correctedValues(r), maxReading);
  }

  for (int r = 0; r < 10; r++) {
    // skip 4 and 5 as they are repeats:
    // if (r == 4 || r == 5) continue;
    normalizedValues(r) = correctedValues(r) / maxReading;
    // print to 5 decimal places:
    Serial.print(correctedValues(r), 5);
    if (r < 9) Serial.print(",");
  }
  Serial.println();
  Serial.println();
  XYZ = corrMatrix * correctedValues;
  for (int i = 0; i < 3; i++) {
    Serial.println(XYZ(i), 5);
  }
}