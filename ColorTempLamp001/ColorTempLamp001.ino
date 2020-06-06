#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 60;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);
unsigned long color = 0xFF;   // start with blue
int times = 0;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);
  delay(3000);
  while (!tcs.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    tcs.begin();
    delay(1000);
  }
  Serial.println("Found sensor");
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop(void) {
  if (times % 100 == 0) {
  for (int p = 0; p < pixelCount; p++) {
    strip.setPixelColor(p, color);// set the color for this pixel
  }
  strip.show();
  }
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

 color = color << 8;         // shift the FF (255) to the next color
  if (color > 0xFF0000) {   // if the color is greater than white (0xFF000000)
    color = 0xFF;             // then set it back to blue
  }
  times++;

}
