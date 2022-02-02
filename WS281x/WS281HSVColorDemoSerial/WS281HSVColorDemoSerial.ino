/*
  WS281x strip using Adafruit_NeoPixel library

  This sketch shows the colors of the HSV wheel
  You can stop it by sending ' ' from the serial monitor
  and restart it by sending any letter from the serial monitor

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  modified 4 July 2019
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels



// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
unsigned int hue;
int sat = 255;
int intensity = 255;
long lastRgbColor = 0;
bool stopped = false;
void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
  Serial.begin(9600);
}

void loop() {

  int rgbColor;
  // a variable to read incoming characters from the serial port:
  char inChar = -1;
  // if there are any characters available to read from the serial port:
  if (Serial.available() > 0) {
    // read the first character to arrive:
    inChar = Serial.read();
    // if the character is a space:
    if (inChar == ' ') {
      stopped = true;
    }
    // if the character is anything from a-z or A-Z:
    if (inChar > 'A') {
      stopped = false;
    }
  }
  // if stopped is false, scroll through the colors:
  if (!stopped)  {
    if (hue < 65536) {
      hue++;
    }
    else {
      hue = 0;
    }

    for (int p = 0; p < pixelCount; p++) {
      // get RGB from HSV:
      unsigned long color = strip.ColorHSV(hue, sat, intensity);
      // do a gamma correction:
      unsigned long correctedColor = strip.gamma32(color);
      strip.setPixelColor(p, correctedColor);
      rgbColor = strip.getPixelColor(p);
    }
    strip.show();
    Serial.print(rgbColor, HEX);
    Serial.print("\t");
    Serial.print(hue);
    Serial.print("\t");

    switch (rgbColor) {
      case 0xFF0000: // 64530 - 1006
        Serial.println("Red");
        break;
      case 0xFF00: //20839-22851
        Serial.println("Green");
        break;
      case 0xFF: // 42685-44697
        Serial.println("blue");
        break;
      case 0xFFFF00: // 10902-10944
        Serial.println("yellow");
        break;
      case 0xFFFF: // 32747 - 32789
        Serial.println("cyan");
        break;
      case 0xFF00FF:  // 54592-54634
        Serial.println("magenta");
        break;
      default:
        Serial.println();
    }
  }
}
