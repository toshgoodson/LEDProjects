#include <Adafruit_NeoPixel.h>
#define PIN 6
#define HAT_NUMBER 2 // 1 or 2

#if HAT_NUMBER == 1
  #define NUM_LEDS 419
  const uint16_t rowCounts[] = {38, 37, 35, 33, 32, 30, 28, 26, 24, 22, 20, 19, 17, 15, 13, 10, 8, 6, 5, 1};
#elif HAT_NUMBER == 2
  #define NUM_LEDS 442//442//409//373
  const uint16_t rowCounts[] = {39, 38, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 12, 10, 8, 6, 4};
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//const uint16_t rowCounts[] = //{39, 37, 36, 35, 33, 31, 29, 27, 25, 23, 21, 19, 18, 15, 14, 12, 9, 8, 6, 4, 1}; //{38, 37, 35, 34, 31, 29, 27, 25, 23, 20, 18, 16, 13, 11, 9, 6, 1 };
const uint16_t numRows = sizeof(rowCounts)/sizeof(rowCounts[0]);
struct VirtualStrip {
  uint16_t start, count;
};
struct VirtualStrip vStrips[numRows];
bool flipFlop = true;
uint16_t counter = 0;

unsigned long frameCounter = 0;

void setup() {
  uint16_t start = 0;
  for (uint16_t i=0; i<numRows; i++) {
    struct VirtualStrip s = {.start = start, .count = rowCounts[i] };
    vStrips[i] = s;
    start = start + rowCounts[i];
  }
//  Serial.begin(9600);
//  while (! Serial);

  randomSeed(27);
  
  strip.begin();
  strip.setBrightness(10);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
//  for(int i = 0; i < numRows; i++) {
//    uint32_t color;
//    switch(i % 3) {
//      case 0:
//        color = 0xff0000;
//        break;
//      case 1:
//        color = 0x00ff00;
//        break;
//      case 2:
//        color = 0x0000ff;
//        break;
//    }
//    setVirtualStripAll(vStrips[i], color);
//  }
//  strip.show();
//  delay(1000000);
//    setAll(255,255,0);
//  delay(1000);
  
//  while(true) {
//   rainbowCycle(20);
//  }
//  setAll(255,255,0);
//  delay(1000);
//  rotatingRingsLoop();
//  for(int i=0; i<5; i++) {
//    rainbowLoop();
//  }

  


  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 255, 255), strip.Color(0, 0, 0));
  }
  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 255, 0), strip.Color(0, 255, 255));
  }
  updateFrameCounters();
  while(true) {
    Fire(10,15,30);
    if (nextFrameWithDelay(60000, &frameCounter)) { break; }
  }
  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 255, 255), strip.Color(0, 0, 0));
  }
  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 127, 64), strip.Color(0, 255, 127));
  }
  for(int i = 0; i < 3; i++) {
      sidewaysStrips(0xff0000, false);
      sidewaysStrips(0xffa500, true);
      sidewaysStrips(0xffff00, false);
      sidewaysStrips(0x00ff00, true);
      sidewaysStrips(0x0000ff, false);
      sidewaysStrips(0x800080, true);
  }
  for(int i = 0; i < 10; i++) {
    sidewaysStrips(0xffcc00, false);
    sidewaysStrips(0xff00ff, true);
    sidewaysStrips(0x99ff00, false);
    sidewaysStrips(0x3300ff, true);
  }
  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 255, 255), strip.Color(0, 0, 0));
  }
  for(int i=0; i<500; i++) {
    twoColorLoop(strip.Color(255, 0, 255), strip.Color(138, 43, 226));
  }
}

void sidewaysStrips(uint32_t color, boolean reverse) {
    for(int j=0; j < 40; j++) {
    for(int i=0; i<numRows; i++) {
      uint16_t count = rowCounts[i];
      uint16_t pixel = j ;
      if (reverse) { pixel = vStrips[i].count - 1 - pixel; }
      setVirtualStripPixel(vStrips[i], pixel, color);

    }
    strip.show();
    delay(20);
  }
}

void updateFrameCounters() {
  frameCounter = millis();
}

boolean nextFrameWithDelay(uint16_t frameLengthMillis, unsigned long *millisAtLastFrame) {
  unsigned long now = millis();
  if (now - *millisAtLastFrame >= frameLengthMillis) {
    *millisAtLastFrame = now;
    return true;
  }
  return false;
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[numRows];
  uint8_t cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < numRows; i++) {
    cooldown = random(0, ((Cooling * 10) / numRows) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= numRows - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(2);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < numRows; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  strip.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (uint8_t Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setVirtualStripAll(vStrips[numRows - 1 - Pixel], 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setVirtualStripAll(vStrips[numRows - 1 - Pixel], 255, heatramp, 0);
  } else if( t192 > 0x05 ) {                               // coolest
    setVirtualStripAll(vStrips[numRows - 1 - Pixel], heatramp, 0, 0);
  } else {
    setVirtualStripAll(vStrips[numRows - 1 - Pixel], 7, 2, 15);
  }
}







void clearStrip() {
  for(uint16_t j=0; j<strip.numPixels(); j++) { // iterate over every LED of the strip, that hasn't lit up yet
      strip.setPixelColor(j, 0x000000);
  }
  strip.show();
}
void randomColorFill(uint8_t wait) {
  clearStrip();
 
  for(uint16_t i=0; i<strip.numPixels(); i++) { // iterate over every LED of the strip
    int r = random(0,255); // generate a random color
    int g = random(0,255);
    int b = random(0,255);
 
    for(uint16_t j=0; j<strip.numPixels()-i; j++) { // iterate over every LED of the strip, that hasn't lit up yet
      strip.setPixelColor(j-1, strip.Color(0, 0, 0)); // turn previous LED off
      strip.setPixelColor(j, strip.Color(r, g, b)); // turn current LED on
      strip.show(); // apply the colors
      delay(wait);
    }
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


int rotatingRingsState[numRows] = {0};
void rotatingRingsLoop() {
  for(uint16_t i=0; i<numRows; i++) {
    struct VirtualStrip s = vStrips[i];
    setVirtualStripPixel(s, rotatingRingsState[i], 0, 255, 255);
    setVirtualStripPixel(s, rotatingRingsState[i]+1, 0, 255, 255);
    setVirtualStripPixel(s, rotatingRingsState[i]+2, 0, 255, 255);
    setVirtualStripPixel(s, rotatingRingsState[i]+3, 0, 255, 255);
  }
  showStrip();
  for(int i=0; i<numRows; i++) {
    struct VirtualStrip s = vStrips[i];
    setVirtualStripPixel(s, rotatingRingsState[i], 0, 0, 0);
    setVirtualStripPixel(s, rotatingRingsState[i]+1, 0, 0, 0);
    setVirtualStripPixel(s, rotatingRingsState[i]+2, 0, 0, 0);
    setVirtualStripPixel(s, rotatingRingsState[i]+3, 0, 0, 0);

  }
  
  // increment/decrement light loop
  for(int j=0;j<numRows;j++) {
    if (j % 2 == 0) {
      rotatingRingsState[j]++;
    } else {
      rotatingRingsState[j] = rotatingRingsState[j] - 1;
    }
    // wrap around
    if (rotatingRingsState[j] < 0) {
      rotatingRingsState[j] = vStrips[j].count - 1;
    }else if(rotatingRingsState[j] >= vStrips[j].count) {
      rotatingRingsState[j] = 0;
    }
    
  }
  delay(20);
}

void rainbowLoop() { 
  for(uint16_t j=0; j<256; j++) {
    for(uint16_t i=0; i<numRows; i++) {
      struct VirtualStrip s = vStrips[i];
      setVirtualStripAll(s, Wheel(((i * 256 / numRows) + j) & 255));
    }
    showStrip();
    delay(20);
  }
}

void twoColorLoop(uint32_t color1, uint32_t color2) {
  uint16_t groupSize = 8;
  
  for(uint16_t i=0; i<numRows; i++) {
    struct VirtualStrip s = vStrips[i];

      if ((counter <= i && i < counter+groupSize) || (counter+groupSize > numRows && i < (counter+groupSize) % numRows)) {
        setVirtualStripAll(s, color1);
      } else {
        setVirtualStripAll(s, color2);
      }
  }
  showStrip();
  delay(20);
  flipFlop = !flipFlop;
  counter++;
  if (counter > numRows) { counter = 0; }
}

void yellowTealLoop() {
  uint16_t groupSize = 8;
  
  for(uint16_t i=0; i<numRows; i++) {
    struct VirtualStrip s = vStrips[i];

      if ((counter <= i && i < counter+groupSize) || (counter+groupSize > numRows && i < (counter+groupSize) % numRows)) {
        setVirtualStripAll(s, 255, 255, 0);
      } else {
        setVirtualStripAll(s, 0, 255, 255);
      }
  }
  showStrip();
  delay(20);
  flipFlop = !flipFlop;
  counter++;
  if (counter > numRows) { counter = 0; }
}

void setVirtualStripPixel(struct VirtualStrip s, int pixel, byte red, byte green, byte blue) {
  if(pixel >= s.count) return;
  setPixel(s.start+pixel, red, green, blue);
}
void setVirtualStripPixelP(struct VirtualStrip* s, int pixel, uint32_t color) {
  strip.setPixelColor(s->start+pixel, color);
}

void setVirtualStripAll(struct VirtualStrip s, byte red, byte green, byte blue) {
  for(uint16_t i = 0; i < s.count; i++ ) {
    setVirtualStripPixel(s, i, red, green, blue); 
  }
}

void setVirtualStripPixel(struct VirtualStrip s, int pixel, uint32_t color) {
  if(pixel >= s.count) return;
  strip.setPixelColor(s.start+pixel, color);
}

void setVirtualStripAll(struct VirtualStrip s, uint32_t color) {
  for(uint16_t i = 0; i < s.count; i++ ) {
    setVirtualStripPixel(s, i, color); 
  }
}


void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
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
