#include <Adafruit_NeoPixel.h>
#define PIN 21
#define NUM_LEDS 2

#define EAR_LEFT 0
#define EAR_RIGHT 1

#define UPDATES_PER_SECOND 60
#define CYCLE_DURATION_BITSHIFT 19 // 2 ^ 19 or 524288 ms = 8.74 minutes
#define NUM_PATTERNS 3

#define BLINK_FRAME_DURATION 30
#define RAINBOW_FADE_HUE_INCREMENT 29
#define RAINBOW_FADE_BRIGHTNESS_INCREMENT 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
unsigned long target_frame_duration_millis = 1000.0 / UPDATES_PER_SECOND;

// complementary rainbow wheel
// rainbow steps one at a time
// same color rainbow wheel
// 7-minute cycles

void setup()
{
	strip.begin();
	strip.setBrightness(64); // 25% brightness seems good in the dark
	strip.show();
}

uint8_t prev_time_bucket = NUM_PATTERNS - 1;
void loop()
{
	uint8_t time_bucket = get_time_bucket(NUM_PATTERNS, CYCLE_DURATION_BITSHIFT);
	// perform pattern variable resets after each pattern
	if (time_bucket != prev_time_bucket) {
		rainbow_wheel_reset();
		rainbow_fade_reset();
		blink_reset();
	}

	switch (time_bucket) {
		case 0:
			rainbow_wheel(127);
			break;
		case 1:
			rainbow_walk(RAINBOW_FADE_HUE_INCREMENT, RAINBOW_FADE_BRIGHTNESS_INCREMENT);
			break;
		case 2:
			rainbow_wheel(0);
			break;
		default:
			blink(0xff0000, BLINK_FRAME_DURATION); // should never reach here
			break;
	}
	strip.show();

	prev_time_bucket = time_bucket;

	delay(target_frame_duration_millis);
}

/***********************************************************************************************************************
 * Divide time into n buckets of t duration and return the bucket number.
***********************************************************************************************************************/
uint8_t get_time_bucket(uint8_t num_buckets, uint8_t bitshift_division) {
	return (millis() >> bitshift_division) % num_buckets;
}

/***********************************************************************************************************************
 * Rainbow color cycle with optional offset for right ear color. 
***********************************************************************************************************************/
uint8_t rainbow_wheel_counter = 0;
void rainbow_wheel(uint8_t right_ear_color_offset)
{
	uint32_t ear_left_color = color_from_wheel(rainbow_wheel_counter);
	uint32_t ear_right_color = color_from_wheel(rainbow_wheel_counter + right_ear_color_offset);

	strip.setPixelColor(EAR_LEFT, ear_left_color);
	strip.setPixelColor(EAR_RIGHT, ear_right_color);
	rainbow_wheel_counter++;
}
void rainbow_wheel_reset() {
	rainbow_wheel_counter = 0;
}

/***********************************************************************************************************************
 * Alternate colors of the rainbow wheel in order starting with the right ear.
***********************************************************************************************************************/
uint8_t rainbow_fade_ear_left_color = 0;
uint8_t rainbow_fade_ear_right_color = 0;
uint8_t rainbow_fade_ear_left_brightness = 255;
uint8_t rainbow_fade_ear_right_brightness = 255;
uint8_t rainbow_fade_darkening = true;
bool rainbow_fading_right = true;
void rainbow_walk(uint8_t wheel_pos_increment, uint8_t brightness_increment) {
	// check if we need to change brightness direction
	if (rainbow_fading_right) {
		if (rainbow_fade_ear_right_brightness == 255)
			rainbow_fade_darkening = true;
		else if (rainbow_fade_ear_right_brightness == 0)
			rainbow_fade_darkening = false;
	} else {
		if (rainbow_fade_ear_left_brightness == 255)
			rainbow_fade_darkening = true;
		else if (rainbow_fade_ear_left_brightness == 0)
			rainbow_fade_darkening = false;
	}

	if (rainbow_fade_darkening) {
		if (rainbow_fading_right) {
			rainbow_fade_ear_right_brightness = capped_subtract(rainbow_fade_ear_right_brightness, brightness_increment);

			if (rainbow_fade_ear_right_brightness == 0) {
				rainbow_fade_ear_right_color += wheel_pos_increment;
			}
		} else {
			rainbow_fade_ear_left_brightness = capped_subtract(rainbow_fade_ear_left_brightness, brightness_increment);

			if (rainbow_fade_ear_left_brightness == 0) {
				rainbow_fade_ear_left_color += wheel_pos_increment;
			}
		}
	} else {
		if (rainbow_fading_right) {
			rainbow_fade_ear_right_brightness = capped_add(rainbow_fade_ear_right_brightness, brightness_increment);

			if (rainbow_fade_ear_right_brightness == 255) {
				rainbow_fading_right = !rainbow_fading_right;
			}
		} else {
			rainbow_fade_ear_left_brightness = capped_add(rainbow_fade_ear_left_brightness, brightness_increment);

			if (rainbow_fade_ear_left_brightness == 255) {
				rainbow_fading_right = !rainbow_fading_right;
			}
		}
	}

	if (rainbow_fading_right) {
		strip.setPixelColor(EAR_LEFT, color_from_wheel(rainbow_fade_ear_left_color)); // full brightness when not walking
		strip.setPixelColor(EAR_RIGHT, color_from_wheel_and_brightness(rainbow_fade_ear_right_color, rainbow_fade_ear_right_brightness));
	} else {
		strip.setPixelColor(EAR_LEFT, color_from_wheel_and_brightness(rainbow_fade_ear_left_color, rainbow_fade_ear_left_brightness));
		strip.setPixelColor(EAR_RIGHT, color_from_wheel(rainbow_fade_ear_right_color)); // full brightness when not walking
	}
}
void rainbow_fade_reset() {
	rainbow_fade_ear_left_color = 0; //hue_from_color(strip.getPixelColor(EAR_LEFT));
	rainbow_fade_ear_right_color = 0; //hue_from_color(strip.getPixelColor(EAR_LEFT));
	rainbow_fade_ear_left_brightness = 255;
	rainbow_fade_ear_right_brightness = 255;
	rainbow_fade_darkening = true;
	rainbow_fading_right = true;
}

/***********************************************************************************************************************
 * Blink on and off for a specified color and number of frames.
***********************************************************************************************************************/
uint8_t blink_counter = 0;
bool blink_on = false;
void blink(uint32_t color, uint8_t step_duration_frames) {
	if (blink_on) {
		strip.setPixelColor(EAR_LEFT, color);
		strip.setPixelColor(EAR_RIGHT, color);
	} else {
		strip.setPixelColor(EAR_LEFT, 0x000000);
		strip.setPixelColor(EAR_RIGHT, 0x000000);
	}

	if (blink_counter >= step_duration_frames ) {
		blink_on = !blink_on;
		blink_counter = 0;
	} else {
		blink_counter++;
	}
}
void blink_reset() {
	blink_counter = 0;
	blink_on = false;
}


/***********************************************************************************************************************
 * Add two numbers without wrapping around.
***********************************************************************************************************************/
uint8_t capped_add(uint8_t a, uint8_t b) {
	uint8_t sum = a + b;
	if (sum < a || sum < b)
		return 255; // if sum is less than either value, the sum wrapped around!
	return sum;
}

/***********************************************************************************************************************
 * Subtract two numbers without wrapping around.
***********************************************************************************************************************/
uint8_t capped_subtract(uint8_t a, uint8_t b) {
	uint8_t diff = a - b;
	if (b == 0)
		return a;
	if (diff >= a)
		return 0; // if diff is greater than or equal to the first value, the diff wrapped around!
	return diff;
}


/***********************************************************************************************************************
 * Get a color from the color wheel (basically HSV with S and V at max).
***********************************************************************************************************************/
uint32_t color_from_wheel(uint8_t wheel_pos)
{
	wheel_pos = 255 - wheel_pos;
	if (wheel_pos < 85)
	{
		return strip.Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
	}
	if (wheel_pos < 170)
	{
		wheel_pos -= 85;
		return strip.Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
	}
	wheel_pos -= 170;
	return strip.Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}

/***********************************************************************************************************************
 * Get a color from the color wheel and adjust brightness (basically HSL with S at max).
***********************************************************************************************************************/
uint32_t color_from_wheel_and_brightness(uint8_t wheel_pos, uint8_t brightness)
{
	wheel_pos = 255 - wheel_pos;
	if (wheel_pos < 85)
	{
		return strip.Color(((255 - wheel_pos * 3) * brightness) >> 8, 0, (wheel_pos * 3 * brightness) >> 8);
	}
	if (wheel_pos < 170)
	{
		wheel_pos -= 85;
		return strip.Color(0, (wheel_pos * 3 * brightness) >> 8, ((255 - wheel_pos * 3) * brightness) >> 8);
	}
	wheel_pos -= 170;
	return strip.Color((wheel_pos * 3 * brightness) >> 8, ((255 - wheel_pos * 3) * brightness) >> 8, 0);
}

/***********************************************************************************************************************
 * Get the hue from a color.
***********************************************************************************************************************/
uint8_t hue_from_color(uint32_t color) {
	uint8_t r = (color & 0xff0000) >> 16;
	uint8_t g = (color & 0x00ff00) >> 8;
	uint8_t b = (color & 0x0000ff);

	uint8_t rgb_min = min(min(r, g), b);
	uint8_t rgb_max = max(max(r, g), b);
	
	if (rgb_min == rgb_max) return 0;

	if (rgb_max == r)
		return 0 + 43 * (g - b) / (rgb_max - rgb_min);
	else if (rgb_max == g)
		return 85 + 43 * (b - r) / (rgb_max - rgb_min);
	else
		return 171 + 43 * (r - g) / (rgb_max - rgb_min);
}