#include <Adafruit_NeoPixel.h>
#include <Math.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define TOSH 1
#define ALLIE 2
#define OWNER TOSH
#define PIN 1
#define NUM_LEDS 19
#define EXIT_LEDS 11
#define SPOTLIGHT_LEDS 8

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

unsigned long frameCounter1 = 0;
uint8_t spotlightStepper = 0;

unsigned long frameCounter2 = 0;
unsigned long frameCounterScene = 0;
uint8_t transitionStepper = 0;


void loop() {  
  cmykSpin(); cmykSpin(); cmykSpin(); cmykSpin(); cmykSpin(); cmykSpin(); cmykSpin(); cmykSpin();
  updateFrameCounters();
  cyanTransitionToRainbow();
  updateFrameCounters();
  rainbow();
  updateFrameCounters();
  transitionToColor(strip.Color(255, 64, 0));
  updateFrameCounters();
  theaterChaseRainbow();
  updateFrameCounters();
  transitionToColor(strip.Color(255, 144, 0));
  updateFrameCounters();
  orangePinkFadeSpin();
  updateFrameCounters();
  transitionToColor(strip.Color(220, 0, 255));
  updateFrameCounters();
  purpleFadeSpin();
  updateFrameCounters();
  greenFadeSpin();
  updateFrameCounters();
  yellowFadeSpin();
  updateFrameCounters();
  niceColors(); niceColors(); niceColors();
  updateFrameCounters();
  transitionToColor(strip.Color(0, 255, 255));
  updateFrameCounters();

//  exitStrip(); 
//  spotlights();
}
void updateFrameCounters() {
  unsigned long mills = millis();
  frameCounter1 = mills;
  frameCounter2 = mills;
  frameCounterScene = mills;
}

void yellowFadeSpin() {
  transitionStepper = 0;
  uint8_t steps = 100;
  while (true) {
    if (nextFrameWithDelay(20, &frameCounter2)) {
      exitFade(strip.Color(255, 187, 0), strip.Color(36, 18, 0), steps, transitionStepper, transitionStepper >= steps);
      transitionStepper = (transitionStepper + 1) % (steps * 2);
    }
    if (nextFrameWithDelay(80, &frameCounter1)) {
      basicX(strip.Color(255, 187, 0), strip.Color(36, 18, 0), strip.Color(92, 47, 0), strip.Color(148, 76, 0), spotlightStepper);
      spotlightStepper = (spotlightStepper + 1) % 4;
      strip.show();
    }
    if (nextFrameWithDelay(30000, &frameCounterScene)) { break; }
  }
}
void greenFadeSpin() {
  transitionStepper = 0;
  uint8_t steps = 100;
  while (true) {
    if (nextFrameWithDelay(20, &frameCounter2)) {
      exitFade(strip.Color(34, 255, 0), strip.Color(0, 97, 27), steps, transitionStepper, transitionStepper >= steps);
      transitionStepper = (transitionStepper + 1) % (steps * 2);
    }
    if (nextFrameWithDelay(80, &frameCounter1)) {
      basicX(strip.Color(34, 255, 0), strip.Color(0, 13, 3), strip.Color(3, 26, 0), strip.Color(0, 97, 27), spotlightStepper);
      spotlightStepper = (spotlightStepper + 1) % 4;
      strip.show();
    }
    if (nextFrameWithDelay(30000, &frameCounterScene)) { break; }
  }
}
void purpleFadeSpin() {
  transitionStepper = 0;
  uint8_t steps = 100;
  while (true) {
    if (nextFrameWithDelay(20, &frameCounter2)) {
      exitFade(strip.Color(220, 0, 255), strip.Color(2, 0, 35), steps, transitionStepper, transitionStepper >= steps);
      transitionStepper = (transitionStepper + 1) % (steps * 2);
    }
    if (nextFrameWithDelay(80, &frameCounter1)) {
      basicX(strip.Color(220, 0, 255), strip.Color(2, 0, 35), strip.Color(17, 0, 66), strip.Color(32, 0, 79), spotlightStepper);
      spotlightStepper = (spotlightStepper + 1) % 4;
      strip.show();
    }
    if (nextFrameWithDelay(30000, &frameCounterScene)) { break; }
  }
}
void exitFade(uint32_t color1, uint32_t color2, uint8_t TotalSteps, uint8_t stage, boolean reverse) {
  stage = stage % TotalSteps;
  if (reverse) {
    stage = TotalSteps - stage;
  }
  uint8_t red = ((Red(color1) * (TotalSteps - stage)) + (Red(color2) * stage)) / TotalSteps;
  uint8_t green = ((Green(color1) * (TotalSteps - stage)) + (Green(color2) * stage)) / TotalSteps;
  uint8_t blue = ((Blue(color1) * (TotalSteps - stage)) + (Blue(color2) * stage)) / TotalSteps;
  exitColor(strip.Color(red, green, blue));
}
uint8_t Red(uint32_t color) {
  return (color >> 16) & 255;
}
uint8_t Green(uint32_t color) {
  return (color >> 8) & 255;
}
uint8_t Blue(uint32_t color) {
  return color & 255;
}
void orangePinkFadeSpin() {
  transitionStepper = 0;
  while (true) {
    if (nextFrameWithDelay(80, &frameCounter1)) {
     switch (transitionStepper / 2) {
        case 0:
          exitColor(strip.Color(255, 144, 0));
          break;
        case 1:
          exitColor(strip.Color(45, 20, 37));
          break;
        case 2:
          exitColor(strip.Color(66, 0, 31));
          break;
        case 3:
          exitColor(strip.Color(132, 40, 1));
          break;
      }
      basicX(strip.Color(255, 144, 0), strip.Color(45, 20, 37), strip.Color(66, 0, 31), strip.Color(132, 40, 1), spotlightStepper);
      spotlightStepper++;
      if (spotlightStepper % 4 == 0) {
        spotlightStepper = 0;
        transitionStepper = (transitionStepper + 1) % 8;
      }
      strip.show();
    }
    if (nextFrameWithDelay(30000, &frameCounterScene)) { break; }
  }
}
void niceColors() {
  transitionToColor(0x009432);
  transitionToColor(strip.Color(234,32,39));
  transitionToColor(0x006266);
  transitionToColor(strip.Color(238,90,36));
  transitionToColor(0xeb1700);
  transitionToColor(0xffcc00);
  transitionToColor(0xa800f0);
}
void cmykSpin() {
  exitColor(strip.Color(0, 255, 255));
  while (true) {
    singleSpin(80, strip.Color(0, 255, 255), &frameCounter1);
    strip.show();
    if (nextFrameWithDelay(1000, &frameCounterScene)) { break; }
  }
  exitColor(strip.Color(255, 0, 255));
  while (true) {
    singleSpin(80, strip.Color(255, 0, 255), &frameCounter1);
    strip.show();
    if (nextFrameWithDelay(1000, &frameCounterScene)) { break; }
  }
  exitColor(strip.Color(255, 255, 0));
  while (true) {
    singleSpin(80, strip.Color(255, 255, 0), &frameCounter1);
    strip.show();
    if (nextFrameWithDelay(1000, &frameCounterScene)) { break; }
  }
  exitColor(strip.Color(255, 255, 255));
  while (true) {
    singleSpin(80, strip.Color(255, 255, 255), &frameCounter1);
    strip.show();
    if (nextFrameWithDelay(1000, &frameCounterScene)) { break; }
  }
}
void cyanTransitionToRainbow() {
  transitionStepper = 0;
  exitColor(strip.Color(0, 255, 255));
  while (true) {
    unsigned long tempCounter = frameCounter1;
    singleSpin(80, strip.Color(0, 255, 255), &frameCounter1);
    boolean finishedWipe = rainbowWipe(80, &tempCounter);
    strip.show();
    if (finishedWipe) { break; }
  }
}
void rainbow() {
  rainbowCycle(20);
}
void transitionToColor(uint32_t color) {
  transitionStepper = 0;
  while (true) {
    boolean finishedWipe = centerWipe(80, color, &frameCounter1, false);
    strip.show();
    if (finishedWipe) { break; }
  }
}
void theaterChaseRainbow() {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(100);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


// wipes
boolean centerWipe(uint8_t d, uint32_t color, unsigned long *millisAtLastFrame, boolean blankSpotlights) {
  uint8_t startUpperLed = EXIT_LEDS / 2;
  uint8_t startLowerLed = (EXIT_LEDS - 1) / 2;
  if (nextFrameWithDelay(d, millisAtLastFrame)) {
    // if we're starting to fill in spotlights, blank them first
    if (startUpperLed + transitionStepper >= EXIT_LEDS && blankSpotlights) {
      spotlightColor(strip.Color(0,0,0));
    }
    for (uint8_t i = 0; i < transitionStepper; i++) {
      if (startUpperLed + i < NUM_LEDS) {
        strip.setPixelColor(startUpperLed + i, color);
      }
      if (startLowerLed - i >= 0) {
        strip.setPixelColor(startLowerLed - i, color);
      }
    }
    if ( startUpperLed + transitionStepper > NUM_LEDS) {
      return true;
    }
    transitionStepper++;
  }
  return false;
}
boolean rainbowWipe(uint8_t d, unsigned long *millisAtLastFrame) {
  uint8_t startUpperLed = EXIT_LEDS / 2;
  uint8_t startLowerLed = (EXIT_LEDS - 1) / 2;
  if (nextFrameWithDelay(d, millisAtLastFrame)) {
    // if we're starting to fill in spotlights, blank them first
    if (startUpperLed + transitionStepper >= EXIT_LEDS) {
      spotlightColor(strip.Color(0,0,0));
    }
    for (uint8_t i = 0; i < transitionStepper; i++) {
      if (startUpperLed + i < NUM_LEDS) {
        strip.setPixelColor(startUpperLed + i, Wheel((((startUpperLed + i) * 256 / NUM_LEDS) ) & 255));
      }
      if (startLowerLed - i >= 0) {
        strip.setPixelColor(startLowerLed - i, Wheel((((startLowerLed - i) * 256 / NUM_LEDS) ) & 255));
      }
    }
    if ( startUpperLed + transitionStepper > NUM_LEDS) {
      return true;
    }
    transitionStepper++;
  }
  return false;
}

// spins
void singleSpin(uint8_t d, uint32_t color, unsigned long *millisAtLastFrame) {
  if (nextFrameWithDelay(d, millisAtLastFrame)) {
    basicX(color, strip.Color(0, 0, 0),  strip.Color(0, 0, 0),  strip.Color(0, 0, 0), spotlightStepper);
    spotlightStepper = (spotlightStepper + 1) % 4;
  }
}

// sets
void exitColor(uint32_t color) {
   for(uint8_t i = 0; i < EXIT_LEDS; i++) {
      strip.setPixelColor(i, color);
    }
}
void spotlightColor(uint32_t color) {
   for(uint8_t i = EXIT_LEDS; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, color);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


boolean nextFrameWithDelay(uint16_t frameLengthMillis, unsigned long *millisAtLastFrame) {
  unsigned long now = millis();
  if (now - *millisAtLastFrame >= frameLengthMillis) {
    *millisAtLastFrame = now;
    return true;
  }
  return false;
}
/*
 * 4 \ / 1          1 \ / 4    
 *    -      Exit      -
 * 3 / \ 2          2 / \ 3
 * 
 * color1 - The color of leg 1
 * color2 - The color of leg 2
 * color3 - The color of leg 3
 * color4 - The color of leg 4
 * step   - Must be [0 - 3]. Steps legs around in circle
 */
void basicX(uint32_t color1, uint32_t color2,  uint32_t color3,  uint32_t color4, uint8_t step) {
  for(uint8_t i = 0; i < step; i++) {
    uint32_t colorX = color4;
    color4 = color3;
    color3 = color2;
    color2 = color1;
    color1 = colorX;
  }
  strip.setPixelColor(11, color1);
  strip.setPixelColor(12, color1);
  strip.setPixelColor(15, color2);
  strip.setPixelColor(16, color2);
  strip.setPixelColor(13, color3);
  strip.setPixelColor(14, color3);
  strip.setPixelColor(17, color4);
  strip.setPixelColor(18, color4);
}

