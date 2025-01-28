/*
  Candle example
  Runs only on SAMD boards (Nano 33 IoT, MKR series)
  uses Adafruit's NeoPixel library and the ColorHSV function therein
  Also uses Scheduler library, which runs only on SAMD boards.

  created  6 Jun 2020
  modified 6 Feb 2023
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <Scheduler.h>

const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel candle = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

int hues[pixelCount];
int saturations[pixelCount];
int intensities[pixelCount];
int changeValues[] = {1, 1, 1, 1, 1, 1, 1};

int highPixels[] = {6, 3};
int lowPixels[] = {1, 4};
int bluePixel = 0;
int lightPixels[] = {2, 5};


void setup() {
  Serial.begin(9600);

  candle.begin();    // initialize pixel strip
  candle.clear();    // turn all LEDs off
  candle.show();     // update strip

  // set all initial hues, sats, intensities, and colorConverters
  for (int p = 0; p < 2; p++) {
    int thisPixel = highPixels[p];
    // between 1200 - 2400:
    hues[thisPixel] = random(1200) + 1200;
    // 240 - 250:
    saturations[thisPixel] = random(10) + 240;
    // 200 - 220:
    intensities[thisPixel] = random(20) + 200;
  }

  for (int p = 0; p < 2; p++) {
    int thisPixel = lowPixels[p];
    hues[thisPixel] = random(800) + 300;
    saturations[thisPixel] = 255;
    intensities[thisPixel] = random(20) + 100;
  }

  for (int p = 0; p < 2; p++) {
    int thisPixel = lightPixels[p];
    hues[thisPixel] = random(1500) + 800;
    saturations[thisPixel] = random(20) + 220;
    intensities[thisPixel] = random(40) + 110;
  }


  hues[bluePixel] = random(200) + 30000;
  saturations[bluePixel] = random(10) + 230;
  intensities[bluePixel] = random(20) + 30;

  // set up some loops for timing:
  Scheduler.startLoop(fastLoop);
  Scheduler.startLoop(medLoop);
  Scheduler.startLoop(slowLoop);
}

void loop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = highPixels[p];

    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but change directions at the extremes:
    if (hues[thisPixel] < 8 || hues[thisPixel] > 18) {
      changeValues[thisPixel] = -changeValues[thisPixel];
    }
    long thisColor = candle.ColorHSV(hues[thisPixel],
                                     saturations[thisPixel],
                                     intensities[thisPixel]);
    candle.setPixelColor(thisPixel, thisColor);
  }
  candle.show();
  delay(50);
  yield();
}


void fastLoop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = lowPixels[p];
    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but add a random -1 to 2:
    hues[thisPixel] += (random(3) - 1);
    hues[thisPixel] = constrain(hues[thisPixel], 4, 16);

    long thisColor = candle.ColorHSV(hues[thisPixel],
                                     saturations[thisPixel],
                                     intensities[thisPixel]);
    candle.setPixelColor(thisPixel, thisColor);
  }
  candle.show();
  delay(30);
}


void medLoop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = lightPixels[p];

    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but change directions at the extremes:
    if (hues[thisPixel] < 4 || hues[thisPixel] > 20) {
      changeValues[thisPixel] = -changeValues[thisPixel];
    }
    long thisColor = candle.ColorHSV(hues[thisPixel],
                                     saturations[thisPixel],
                                     intensities[thisPixel]);
    candle.setPixelColor(thisPixel, thisColor);
  }
  candle.show();
  delay(60);
}

void slowLoop() {
  // change the hue:
  hues[bluePixel] = hues[bluePixel] + changeValues[bluePixel];
  // change the intensity and constrain it:
  intensities[bluePixel] += (random(3) - 1);
  intensities[bluePixel] = constrain(intensities[bluePixel], 0, 20);

  // keep the change within the min/max range,
  // but change directions at the extremes:
  if (hues[bluePixel] < 200 || hues[bluePixel] > 280) {
    changeValues[bluePixel] = -changeValues[bluePixel];
  }
  long thisColor = candle.ColorHSV(hues[bluePixel],
                                   saturations[bluePixel],
                                   intensities[bluePixel]);
  candle.setPixelColor(bluePixel, thisColor);
  candle.show();
  delay(100);
  yield();
}
