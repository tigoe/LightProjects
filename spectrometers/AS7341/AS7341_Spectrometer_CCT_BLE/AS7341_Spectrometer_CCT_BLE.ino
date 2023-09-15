/*
  This sketch calculates lux, CCT, x, y, and z  from the basic count readings of an
  AMS AS7341 sensor.  
  
  This is a first draft and needs corrections. Readings do not correlate
  with a Sekonic C-800-U measuring next to the sensor. 
  The sensor is diffused with a single sheet of Kimoto Optsaver L-57 diffuser.

  It sends the following out as CSV string:

{"415": // 0.0-1.0,
 "445":// 0.0-1.0,
 "480":// 0.0-1.0,
 "515":// 0.0-1.0,
 "555":// 0.0-1.0,
 "590":// 0.0-1.0,
 "630":// 0.0-1.0,
 "680":// 0.0-1.0,
 "910":// 0.0-1.0,
 "clear":// 0.0-1.0,
 "x":// 0.0-1.0,
 "y":// 0.0-1.0,
 "z":// 0.0-1.0,
 "lux":// lx,
 "cct":// cct,
 "flicker": // Hz}

  Libraries used:
  http://librarymanager/All#Arduino_JSON
  http://librarymanager/All#Adafruit_AS7341
  http://librarymanager/All#BasicLinearAlgebra

  See calibration methods doc and spreadsheet from AMS for more detail:
  https://ams.com/documents/20143/36005/AS7341_AN000633_1-00.pdf/fc552673-9800-8d60-372d-fc67cf075740
  https://ams.com/documents/20143/36005/AS7341_AD000198_3-00.xlsx
 
  created 2 May 2023
  by Tom Igoe
*/

#include <BasicLinearAlgebra.h>
#include <Adafruit_AS7341.h>
#include <ArduinoBLE.h>

// the uuid for the service. To make the uuids for the characteristics,
// replace the 0000 with higher values (0001, etc):
char serviceUuid[] = "9af01fc3-0000-44b8-8acc-f3ed7a225431";
char characteristicUuid[] = "9af01fc3-0001-44b8-8acc-f3ed7a225431";

// fill in your device name here"
char bleName[] = "spectroscope";
// readingString will be 410 bytes in length:
const int readingLength = 200;

// BLE service and characteristic:
BLEService spectroService(serviceUuid);
// create sensor characteristic and allow remote device to get notifications:
BLECharacteristic spectroCharacteristic(characteristicUuid, BLERead | BLENotify, readingLength);


// use the BasicLinearAlgebra namespace.
//  You might need to add BLA::  before all the Matrix declarations
using namespace BLA;

// instance of the sensor library:
Adafruit_AS7341 as7341;
// array to hold the raw readings:
// uint16_t readings[12];
// string for readings:
String readingString;
// properties of the object (the channels of the sensor):
int wavelengths[] = { 415, 445, 480, 515, 555, 590, 630, 680, 910 };

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

void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);

  // use builtin LED for connection indicator:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  // readingString will need 50 bytes for all values:
  readingString.reserve(readingLength);


  if (!as7341.begin()) {
    digitalWrite(2, HIGH);
    Serial.println("Sensor not found, check wiring");
    while (true)
      ;
  }
  // set sensor integration time, etc:
  as7341.setATIME(35);
  as7341.setASTEP(10000);
  as7341.setGain(AS7341_GAIN_256X);
  // start a new reading cycle:
  as7341.startReading();

  // begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed");
    while (true)
      ;
  }

  // set the local name peripheral advertises:
  BLE.setLocalName(bleName);
  // print it:
  Serial.println(bleName);
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(spectroService);
  // add the characteristic:
  spectroService.addCharacteristic(spectroCharacteristic);
  // add the service:
  BLE.addService(spectroService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // convert readingString to a char array set the value:
  spectroCharacteristic.writeValue(readingString.c_str());

  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events:
  BLE.poll();

  // read the sensor, print if there are good values:
  if (as7341.checkReadingProgress()) {
    if (readSensor()) {
      calculateLightValues();
      // if a central subscribes to the spectro characteristic:
      if (spectroCharacteristic.subscribed()) {
        Serial.println("subscribed");
        // update the characteristic:
        spectroCharacteristic.writeValue(readingString.c_str());
      }
      if (Serial) {
        Serial.println(readingString.c_str());
      }
    }
    // start a new reading cycle:
    as7341.startReading();
  }
}


void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  // turn on the builtin LED when connected:
  digitalWrite(LED_BUILTIN, HIGH);
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  // turn off the builtin LED when disconnected:
  digitalWrite(LED_BUILTIN, LOW);
}

bool readSensor() {
  // check to see if readings are done
  uint16_t data[12];

  // if you get no readings, return:
  if (!as7341.readAllChannels(data)) return false;
  // there are ten channels, eight of which are visible bands
  // but two channels, the NIR and clear, repeat.
  // so we need to read from the 12 channels into 10 places of a data array:
  int c = 0;

  // loop over the readings and convert them:
  for (int r = 0; r < 12; r++) {
    // skip 4 and 5,  they repeat other channels:
    if (r == 4 || r == 5) continue;
    // after channel 5, c = r - 2 because of the skip:
    readings(c) = as7341.toBasicCounts(data[r]);
    // increment channel counter:
    c++;
  }
  return true;
}

void calculateLightValues() {
  // clear readingString:
  readingString = "";
  // XYZ readings:
  Matrix<3> XYZ;
  // zero out the corrected and normalized matrices:
  correctedValues.Fill(0);
  normalizedValues.Fill(0);
  // max reading for the normalization:
  float maxReading = 0.0;
  // loop over the readings and find the max:
  for (int r = 0; r < 10; r++) {
    correctedValues(r) = correctionFactors(r) * (readings(r) - offsets(r));
    maxReading = max(correctedValues(r), maxReading);
    // use it to calculate normalized values:
    normalizedValues(r) = correctedValues(r) / maxReading;

    // give the lightReadings JSON elements names from the properties array,
    // and give them values from the correctedValues matrix:
    String myWavelength = String(wavelengths[r]);


    if (r < 10) {
      readingString += String(correctedValues(r));
    }
    if (r < 11) readingString += ",";
  }

  // from spreadsheet , tab: demonstration calculations, J10-12:
  XYZ = corrMatrix * correctedValues;
  // from spreadsheet, tab: demonstration calculations, J14-16:
  float XYZSum = XYZ(0) + XYZ(1) + XYZ(2);
  float x = XYZ(0) / XYZSum;
  float y = XYZ(1) / XYZSum;
  float z = XYZ(2) / XYZSum;

  // not clear on the origin of the constants below:
  // from spreadsheet, tab: demonstration calculations, J17:
  float lux = XYZ(1) * 683.0;
  readingString += String(lux);
  readingString += ",";
  // from spreadsheet, tab: demonstration calculations, J22:
  float cct = 437 * pow(((x - 0.332) / (0.1858 - y)), 3) + 3601 * pow(((x - 0.332) / (0.1858 - y)), 2) + 6861 * ((x - 0.332) / (0.1858 - y)) + 5517;
  readingString += String(cct);
  readingString += ",";
  // add flicker detection:
  int flickerHz = as7341.detectFlickerHz();
  readingString += String(flickerHz);
}
