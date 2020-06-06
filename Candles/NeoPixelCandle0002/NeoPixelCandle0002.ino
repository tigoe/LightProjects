#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
#include <Scheduler.h>

const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel candle = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;

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
    hues[thisPixel] = random(10) + 8;
    saturations[thisPixel] = random(2) + 98;
    intensities[thisPixel] = random(20) + 70;
  }

  for (int p = 0; p < 2; p++) {
    int thisPixel = lowPixels[p];
    hues[thisPixel] = random(12) + 3;
    saturations[thisPixel] = 100;
    intensities[thisPixel] = random(20) + 30;
  }

  for (int p = 0; p < 2; p++) {
    int thisPixel = lightPixels[p];
    hues[thisPixel] = random(16) + 4;
    saturations[thisPixel] = random(10) + 80;
    intensities[thisPixel] = random(20) + 30;
  }


  hues[bluePixel] = random(40) + 240;
  saturations[bluePixel] = random(5) + 95;
  intensities[bluePixel] = random(10) + 5;


  // set up some loops for timing:
  Scheduler.startLoop(fastLoop);
  Scheduler.startLoop(medLoop);
  Scheduler.startLoop(slowLoop);
}

void loop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = highPixels[p];
    RGBColor colors = converter.HSItoRGBW(hues[thisPixel],
                                            saturations[thisPixel],
                                            intensities[thisPixel]);

    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but change directions at the extremes:
    if (hues[thisPixel] < 8 || hues[thisPixel] > 18) {
      changeValues[thisPixel] = -changeValues[thisPixel];
    }
    candle.setPixelColor(thisPixel,
                         colors.red,
                         colors.green,
                         colors.blue,
                         colors.white);
  }
  candle.show();
  delay(50);
  yield();
}


void fastLoop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = lowPixels[p];
    RGBColor colors = converter.HSItoRGBW(hues[thisPixel],
                                            saturations[thisPixel],
                                            intensities[thisPixel]);

    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but add a random -1 to 2:
    hues[thisPixel] += (random(3) - 1);
    hues[thisPixel] = constrain(hues[thisPixel], 4, 16);

    candle.setPixelColor(thisPixel,
                         colors.red,
                         colors.green,
                         colors.blue,
                         colors.white);
  }
  candle.show();
  delay(30);
}


void medLoop() {
  for (int p = 0; p < 2; p++) {
    int thisPixel = lightPixels[p];
    RGBColor colors = converter.HSItoRGBW(hues[thisPixel],
                                            saturations[thisPixel],
                                            intensities[thisPixel]);

    // change the hue:
    hues[thisPixel] = hues[thisPixel] + changeValues[thisPixel];
    // keep the change within the min/max range,
    // but change directions at the extremes:
    if (hues[thisPixel] < 4 || hues[thisPixel] > 20) {
      changeValues[thisPixel] = -changeValues[thisPixel];
    }
    candle.setPixelColor(thisPixel,
                         colors.red,
                         colors.green,
                         colors.blue,
                         colors.white);
  }
  candle.show();
  delay(60);
}

void slowLoop() {
  RGBColor colors = converter.HSItoRGBW(hues[bluePixel],
                                          saturations[bluePixel],
                                          intensities[bluePixel]);

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

  candle.setPixelColor(bluePixel,
                       colors.red,
                       colors.green,
                       colors.blue,
                       colors.white);
  candle.show();
  delay(100);
  yield();
}
