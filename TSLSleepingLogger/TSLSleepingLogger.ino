/*
  TMP36 SD datalogger  with sleep functionality
  Saves data from two analog inputs to an SD card and shows how to
  use an alarm to wake an M0-based board  from standby mode.
  Works only on M0-based boards.

  Note that the USB serial port will disappear while the board is in standby mode.
  To bring the board out of standby mode, double-tap the reset button.

  Serial printing happens only if the serial port is open on your computer
  (that's what if (Serial) does).

  RTC code based on Arturo Guadalupi's RTC sleep example
  created 20 Feb 2018
  updated 24 Feb 2018
  by Tom Igoe
*/

#include <RTCZero.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591();

const int chipSelect = SDCARD_SS_PIN;   // CS pin for the SD card. For MKRZero use SDCARD_SS_PIN
volatile bool logging = false;          // flag to tell main loop to log data
RTCZero rtc;                // realtime clock
int state = LOW;            // LED state
bool cardPresent = false;   // if the card is present

void setup() {
  Serial.begin(9600);
  analogReference(AR_DEFAULT);
  
  Serial.println(__DATE__);         // print date of compile
  Serial.println(__TIME__);         // print time of compile
  pinMode(LED_BUILTIN, OUTPUT);     // set builtin LED to output
  digitalWrite(LED_BUILTIN, HIGH);  // set it high

  if (tsl.begin()) {
    // gain can be low (1x), med(25x), high(428x):
    tsl.setGain(TSL2591_GAIN_MED);

    // integration time can be 100, 200, 300, 400, 500, 600ms.
    // longer integration gathers more light:
    tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  } else {
    Serial.println("No sensor found. check your wiring.");
    while (true);     // stay here and do nothing
  };

  // see if the card is present and can be initialized:
  if (!SD.begin()) {
    if (Serial) Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  
  cardPresent = true;
  if (Serial) Serial.println("card initialized.");

  rtc.begin();                      // start the realtime clock
  rtc.setAlarmTime(00, 00, 00);        // set an alarm for processor wakeup
  rtc.enableAlarm(rtc.MATCH_SS);  // enable it for once a second
  rtc.attachInterrupt(alarmMatch);  // have the wakeup happen on the alarm
}

void loop() {
  if (logging) {                          // if you need to log,
    String timestamp = getTimeStamp();    // get the timestamp
    unsigned long luxNow = readLux();    // get the lux now
    if (cardPresent) {                    // if the SD card is present
      writeToCard(luxNow, timestamp);    // write temp and time to SD card
    }
    logging = false;                      // clear the logging flag
    rtc.enableAlarm(rtc.MATCH_SS);        // enable alarms again
  }

  // if not logging, go to sleep on the 10th second of the minute:
  if (rtc.getSeconds() == 10 && !logging) {
    if (Serial) Serial.println("going into standby");
    rtc.standbyMode();
  }

  // to set the date and time (epoch, or seconds since 1/1/1970) from a POSIX machine, type:
  // $ date +D%DT%s
  // thanks to Paul Stoffregen for this trick

  while (Serial.available()) {
    digitalWrite(LED_BUILTIN, HIGH);    // turn on LED to signal time reading
    // if you get the D
    if (Serial.findUntil("D", "T")) {
      rtc.setMonth(Serial.parseInt());  // set the month, day, year
      rtc.setDay(Serial.parseInt());
      rtc.setYear(Serial.parseInt());
    } // if you get the T:
    if (Serial.findUntil("T", "\n")) {
      unsigned long epoch = Serial.parseInt();  // set the epoch
      rtc.setEpoch(epoch);
      Serial.println(getTimeStamp());   // when done, print the timestamp
    }
  }
  digitalWrite(LED_BUILTIN, LOW);       // turn off LED to signal end of time reading
}

void alarmMatch() {
  logging = true;       // set the logging flag so loop knows to log
  rtc.disableAlarm();   // disable alarms
}

String getTimeStamp() {
  // make an ISO8601 formatted timestring:
  //  e.g. 2018 - 02 - 21T01: 49: 51.296Z
  String timestamp = "20";
  timestamp += rtc.getYear();
  timestamp += "-";
  if (rtc.getMonth() <= 9) timestamp += "0";
  timestamp += rtc.getMonth();
  timestamp += "-";
  if (rtc.getDay() <= 9) timestamp += "0";
  timestamp += rtc.getDay();
  timestamp += "T";
  if (rtc.getHours() <= 9) timestamp += "0";
  timestamp += rtc.getHours();
  timestamp += ":";
  if (rtc.getMinutes() <= 9) timestamp += "0";
  timestamp += rtc.getMinutes();
  timestamp += ":";
  if (rtc.getSeconds() <= 9) timestamp += "0";
  timestamp += rtc.getSeconds();
  timestamp += "Z";
  return timestamp;
}

bool writeToCard(float sensorData, String datetime) {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // this function will make a new file every day
  // filename is 8.3: 8chars, 3 chars in extension max.
  String fileName = "20";
  fileName += rtc.getYear();
  if (rtc.getDay() < 10) fileName += "0";
  fileName += rtc.getDay();
  if (rtc.getMonth() < 10) fileName += "0";
  fileName += rtc.getMonth();
  fileName += ".txt";
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    if (Serial) Serial.println(datetime);
    digitalWrite(LED_BUILTIN, HIGH);      // turn on LED to signal that you're writing
    dataFile.print(datetime);
    dataFile.print(", ");
    dataFile.print(sensorData);
    dataFile.println(" lux");
    dataFile.close();
    digitalWrite(LED_BUILTIN, LOW);      // turn off LED after writing
    return true;
  }
  // if the file isn't open, pop up an error:
  else {
    if (Serial) {
      Serial.print("error opening  ");
      Serial.println(fileName);
    }
    return false;
  }
}

unsigned long readLux() {
  unsigned long luminosity = tsl.getFullLuminosity();
  unsigned int ir = luminosity >> 16;           // IR is the bottom two bytes
  unsigned int full = luminosity & 0xFFFF;      // full is the top two bytes
  unsigned int visible = full - ir;             // visible is full - IR
  unsigned long lux = tsl.calculateLux(full, ir);// calculate lux from luminosity values
  return lux;
}
