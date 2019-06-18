/*
  WWA WS281x strip using ColorConverter

  This sketch uses the Adafruit NeoPixel library to drive a strip of
  WS281x warm white-cool white-amber (WWA) LEDs
  and the ColorConverter library to do warm-to-cool conversion.
  A potentimeter on A0 fades the strip from warm to cool.

  created 11 Feb 2019
  modified 17 June 2019
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
#include <Encoder.h>

const int neoPixelPin = 4;
const int pixelCount = 20;    // number of pixels

// set up strip. Note BRG setting. You might have to experiment
// to determine which way your LEDs are wired:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_BRG + NEO_KHZ800);
ColorConverter converter;

int mix[] = {0, 100, 100};   // array for color temp, saturation, intensity

Encoder knob(0, 1);
int lastPosition = 0;
const int buttonPin = 5;
int lastButtonState = HIGH;

int currentProp = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
  pinMode(buttonPin, INPUT_PULLUP);
  knob.write(0);
}

void loop() {
  // read the sensors:
  int buttonState = digitalRead(buttonPin);
  int encoderPos = knob.read();

  // if the button has changed:
  if (buttonState != lastButtonState) {
    // debounce the button:
    delay(10);
    // if button is pressed:
    if (buttonState == LOW) {
      // increment currentProp and make sure it
      // stays in the 0-2 range:
      currentProp++;
      currentProp %= 3;
    }
    // save current state for next time through the loop:
    lastButtonState = buttonState;
  }

  // calculate direction of change of the encoder:
  int encoderChange = (encoderPos - lastPosition) / 4;

  if (encoderChange != 0) {
    // save current position for next time
    lastPosition = encoderPos;
    // add the change to the value of the current property:
    mix[currentProp] += encoderChange;
    // constrain differently depending on whether it's CT, sat, ir intensity:
    switch (currentProp) {
      // constraon the result to 0 - 240
      // (not 360, because you want warm to cool, not
      // warm to cool and back:
      case 0:   //CT
        mix[currentProp] = constrain(mix[currentProp], 0, 240);
        break;
      // constrain sat and intensity to 0-100:
      case 1:   // sat
      case 2:   // intensity
        mix[currentProp] = constrain(mix[currentProp], 0, 100);
        break;
    }
    Serial.println(String(currentProp) + "," + String(mix[currentProp]));

    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(mix[0], mix[1], mix[2]);

    // change the ColorConverter labels to something you can use:
    int amber = color.red;
    int warm = color.green;
    int cool = color.blue;

    // loop over all the pixels:
    for (int pixel = 0; pixel < pixelCount; pixel++) {
      // set the color for this pixel:
      strip.setPixelColor(pixel, amber, warm, cool);
      Serial.println(String(amber) + "," + String(warm) + "," + String(cool));
    }
    // update the strip
    strip.show();
  }
}
