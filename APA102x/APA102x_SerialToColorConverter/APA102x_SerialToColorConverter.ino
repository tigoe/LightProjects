/*
   Serial to color converter for APA102x LEDs

   This sketch lets you test the color converter by setting a string of
   APA102/APA102C LEDs. To do so, open the serial monitor
   and send one of these strings:

   h xx xx xx\n
   A hue, saturation, intensity string that will return the corresponding RGB values
   hue: 0-360, saturation: 0-100, intensity: 0-100

   r h xx xx xx\n
   A red, green, blue string that will return the corresponding HSI values
   red: 0-255, green: 0-255, blue: 0-255

   created 26 June 2018
   modified 17 Jun 2019
   by Tom Igoe
*/

//#include <ColorConverter.h>
#include <Adafruit_DotStar.h>

const int pixelCount = 4;    // number of pixels

// you need these only if you're doing software SPI:
const int dataPin = 8;      // data pin
const int clockPin = 9;     // clock pin

// set up strip:
//Adafruit_DotStar strip(pixelCount, dataPin, clockPin, DOTSTAR_BRG);
Adafruit_DotStar strip(pixelCount, DOTSTAR_BGR);

//ColorConverter converter;
void setup() {
  Serial.begin(9600);       // initialize serial communication
  while (!Serial);          // wait until serial monitor is opened
  Serial.setTimeout(10);    // set timeout for parseInt() to 10 ms
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {
  if (Serial.available()) {   // if there's any serial data,
    char c = Serial.read();   // read one byte

    if (c == 'h') {       // if byte was h:
      int h = Serial.parseInt();    // read ASCII numeric strings
      int s = Serial.parseInt();
      int i = Serial.parseInt();
      long thisColor = strip.ColorHSV(h, s, i);
      // do the conversion:
//      RGBColor color = converter.HSItoRGB(h, s, i);
//      // print the results:
//      Serial.print(color.red);
//      Serial.print(" ");
//      Serial.print(color.green);
//      Serial.print(" ");
//      Serial.println(color.blue);
      for (int pixel = 0; pixel < pixelCount; pixel++) {
        strip.setPixelColor(pixel, thisColor);   // set the color for this pixel
      }
    }
    if (c == 'r') {      // if byte was r:
      float r = Serial.parseInt();    // read ASCII numeric strings
      float g = Serial.parseInt();
      float b = Serial.parseInt();
      // do the conversion:
      HSIColor color = converter.RGBtoHSI(r, g, b);
      // print the results:
      Serial.print(color.hue);
      Serial.print(" ");
      Serial.print(color.saturation);
      Serial.print(" ");
      Serial.println(color.intensity);
      for (int pixel = 0; pixel < pixelCount; pixel++) {
        strip.setPixelColor(pixel, r, g, b);   // set the color for this pixel
      }
    }
    Serial.println(); // add a blank line
    strip.show();    // refresh the strip
  }
}
