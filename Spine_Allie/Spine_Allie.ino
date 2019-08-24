#include <FastLED.h>

#define ALLIE 0
#define TOSH 1

#define JACKET_TARGET ALLIE

#if JACKET_TARGET == ALLIE
  #define REAL_LEDS 29 // the number of physical leds
  #define NUM_LEDS 15 // the number of leds we actually intend to use 
#elif JACKET_TARGET == TOSH
  #define REAL_LEDS 43 // the number of physical leds
  #define NUM_LEDS 23 // the number of leds we actually intend to use 
#endif

#define DATA_PIN 1
#define LED_TYPE NEOPIXEL
#define BRIGHTNESS 255
#define CYCLE_LENGTH 60000 // in milliseconds
#define CYCLE_STATES 3

CRGB realleds[REAL_LEDS];
CRGB leds[NUM_LEDS];

uint16_t synctime = 0;
uint8_t cycle_state = 0;


#if JACKET_TARGET == ALLIE
void my_map_real_leds() {
  // mapping is: every other led
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    realleds[i*2] = leds[i];
  }
}
#elif JACKET_TARGET == TOSH
void my_map_real_leds() {
  for(uint8_t i = 0; i < NUM_LEDS-3; i++) {
    realleds[i*2] = leds[i];
  }
  realleds[40] = leds[20];
  realleds[41] = leds[21];
  realleds[42] = leds[22];
}
#endif


void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN>(realleds, REAL_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
  clear_real();
  FastLED.show();
}

void clear_real() {
  for(uint8_t j=0; j < REAL_LEDS; ++j) {
    realleds[j] = 0x000000;
  }
}

void clear_all() {
  for(uint8_t j=0; j < NUM_LEDS; ++j) {
    leds[j] = 0x000000;
  }
}

void white_all() {
  for(uint8_t j=0; j < NUM_LEDS; ++j) {
    leds[j] = 0xffffff;
  }
}

void rand_all() {
  uint8_t r = random(255);
  uint8_t g = random(255);
  uint8_t b = random(255);
  for(uint8_t j=0; j < NUM_LEDS; ++j) {
    leds[j].r = r;
    leds[j].g = g;
    leds[j].b = b;
  }
}

void loop() {
  if (cycle_state == 2) {
    my_rainbow();
  } else if (cycle_state == 1) {
    my_strobe();
  } else {
    my_gradient_comet();
  }

  my_map_real_leds();
  FastLED.show();

  synctime++;
  if ( synctime % CYCLE_LENGTH == 0 ) {
    cycle_state = (cycle_state + 1) % CYCLE_STATES;
    synctime = 0;
  }

} 

boolean do_frame(uint8_t fps) {
  if ((synctime) % (1000 / fps) == 0) {
    return true;
  }
  return false;
}

/***
 * Rainbow stuff
 */
#define NUM_RAINBOWS 0.5
#define RAINBOW_FPS  180
uint8_t rainbowCount = 0;
void my_rainbow() {
  if(!do_frame(RAINBOW_FPS)) return;
  
  fill_rainbow(leds, NUM_LEDS, rainbowCount, 255 / NUM_LEDS * NUM_RAINBOWS);
  rainbowCount = (rainbowCount + 1) % 255;
}

/***
 * Strobe stuff
 */
#define STROBE_FPS 6//120
#define STROBE_ON_MOD 1//6 // on every 6th frame
boolean strobe_state = true;
uint8_t strobe_count = 0;
void my_strobe() {
  if(!do_frame(STROBE_FPS)) return;
  
  if(strobe_count % STROBE_ON_MOD == 0) {
    rand_all();
  } else {
    clear_all();
  }
  strobe_count++;
}

/***
 * Downward Strobe stuff
 */
#define DOWNWARD_STROBE_FPS 1
uint8_t downward_strobe_state = 2;
void my_downward_strobe() {
  if(!do_frame(10)) return;

  for(uint8_t i=0; i < NUM_LEDS; i++) {
    if (i % 3 == 0) {
      leds[(downward_strobe_state + i) % NUM_LEDS] = 0xffffff;
    } else {
      leds[(downward_strobe_state + i) % NUM_LEDS] = 0x000000;
    }
  }
  if (downward_strobe_state == 0) {
    downward_strobe_state = 2;
  } else {
    downward_strobe_state--;
  }
}

/***
 * Gradient Comet
 */
#define COMET_LENGTH 8
#define COMET_FPS 20
#define COMET_STATES 5
int8_t comet_step = -COMET_LENGTH;
uint8_t comet_state = 0;

CRGB comet_leds[COMET_LENGTH];

void my_gradient_comet() {
  if(!do_frame(50)) return;

  if (comet_state == 4) {
    // pink to blue tail with orange head
    fill_gradient(comet_leds, 0, CHSV(128, 50, 255), 5, CHSV(255, 255, 255), FORWARD_HUES);
    fill_gradient(comet_leds, 6, CHSV(40, 255, 255), 7, CHSV(40, 255, 255), FORWARD_HUES);
  } else if (comet_state == 3) {
    // purple tail with green head
    fill_gradient(comet_leds, 0, CHSV(194, 50, 255), 5, CHSV(194, 255, 255), FORWARD_HUES);
    fill_gradient(comet_leds, 6, CHSV(90, 175, 255), 7, CHSV(80, 175, 255), FORWARD_HUES);
  } else if(comet_state == 2) {
    // red tail with blue head
    fill_gradient(comet_leds, 0, CHSV(0, 255, 255), 5, CHSV(40, 255, 255), FORWARD_HUES);
    fill_gradient(comet_leds, 6, CHSV(110, 255, 255), 7, CHSV(128, 255, 255), FORWARD_HUES);
  } else if(comet_state == 1) {
    // blue tail with purple head
    fill_gradient(comet_leds, 0, CHSV(128, 50, 255), 5, CHSV(128, 255, 255), FORWARD_HUES);
    fill_gradient(comet_leds, 6, CHSV(200, 255, 255), 7, CHSV(220, 255, 255), FORWARD_HUES);
  } else {
    // yellow tail with red head
    fill_gradient(comet_leds, 0, CHSV(60, 175, 255), 6, CHSV(60, 255, 255), FORWARD_HUES);
    fill_gradient(comet_leds, 6, CHSV(30, 255, 255), 7, CHSV(250, 255, 255), BACKWARD_HUES);
  }  

  clear_all();
  for(uint8_t i=0; i < COMET_LENGTH; i++) {
    if (i + comet_step >= 0 && i + comet_step < NUM_LEDS) {
      leds[i + comet_step] = comet_leds[i];
    }
  }

  comet_step++;
  if (comet_step > NUM_LEDS + COMET_LENGTH) {
    comet_step = -COMET_LENGTH;
    comet_state = (comet_state + 1) % COMET_STATES;
  }
}

