/*
  AS7341 sensor readings.  Reads and sends the results via BLE
  as a CSV string. This version uses a non-blocking
  approach, using the startReading() and checkReadingProgress()
  functions. It's about 2 seconds between readings.

  Updated to use the BLE event handlers to manage the central connection.

  Library:
  http://librarymanager/All#Adafruit_AS7341
  http://librarymanager/All#ArduinoBLE

  created 18 Jun 2021
  modified 6 Mar 2022
  by Tom Igoe
*/

#include <Adafruit_AS7341.h>
#include <ArduinoBLE.h>

// the uuid for the service. To make the uuids for the characteristics,
// replace the 0000 with higher values (0001, etc):
char serviceUuid[] = "9af01fc3-0000-44b8-8acc-f3ed7a225431";
char characteristicUuid[] = "9af01fc3-0001-44b8-8acc-f3ed7a225431";

// fill in your device name here"
char bleName[] = "spectroscope";

// instance of the sensor library:
Adafruit_AS7341 as7341;

// array to hold the raw readings:
uint16_t readings[12];
// readingString will be 50 bytes in length:
const int readingLength = 50;
// string for readings:
String readingString;

// BLE service and characteristic:
BLEService spectroService(serviceUuid);
// create sensor characteristic and allow remote device to get notifications:
BLECharacteristic spectroCharacteristic(characteristicUuid, BLERead | BLENotify, readingLength);

void setup() {
  // init serial, wait 3 secs for serial monitor to open:
  Serial.begin(9600);
  // if the serial port's not open, wait 3 seconds:
  if (!Serial) delay(3000);
  // use builtin LED for connection indicator:
  pinMode(LED_BUILTIN, OUTPUT);

  // readingString will need 50 bytes for all values:
  readingString.reserve(readingLength);

  pinMode(2, OUTPUT);

  if (!as7341.begin()) {
    digitalWrite(2, HIGH);
    Serial.println("Sensor not found, check wiring");
    while (true);
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
    while (true);
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

  // if you get a good sensor reading:
  if (readSensor()) {
    // if a central subscribes to the spectro characteristic:
    if (spectroCharacteristic.subscribed()) {
      Serial.println("subscribed");
      // update the characteristic:
      spectroCharacteristic.writeValue(readingString.c_str());
    }
    Serial.println(readingString);
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
