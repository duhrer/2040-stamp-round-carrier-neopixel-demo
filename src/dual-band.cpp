#include <math.h>
#include <string.h>
#include <time.h>

#include <stdio.h>

#include "Adafruit_NeoPixel.hpp"

#include "colour-wheel.h"


// See: https://lectronz.com/products/rp2040-stamp-round-carrier
#define PIN 24
#define NUMPIXELS 16

#define LESS_BRIGHT_GPIO_PIN 14
#define MORE_BRIGHT_GPIO_PIN 15

# define TICK_MS 50
# define ROTATION_TICKS 10

# define BAND_WIDTH 9

int ticks = 0;

bool keep_running = true;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup () {
    // NeoPixel Init
    pixels.begin();
    pixels.clear();
    pixels.show();

    // Brightness Button Init
    gpio_init(LESS_BRIGHT_GPIO_PIN);

    // Required for "active low" buttons like this.
    gpio_pull_up(LESS_BRIGHT_GPIO_PIN);

    gpio_init(MORE_BRIGHT_GPIO_PIN);

    // Required for "active low" buttons like this.
    gpio_pull_up(MORE_BRIGHT_GPIO_PIN);

    stdio_init_all();
}

uint less_button_value = 0;
uint more_button_value = 0;
bool button_held = false;

# define MAX_DOT_INTENSITY 255
# define MIN_DOT_INTENSITY 32
# define DOT_INTENSITY_INCREMENT ((MAX_DOT_INTENSITY - MIN_DOT_INTENSITY)/NUMPIXELS)

int dot_intensity = 128;

int r = 0;
int g = 0;
int b = 255;

int target_colour_index = 0;
int positive_dot_index = 0;
int negative_dot_index = NUMPIXELS / 2;

void poll_button_values () {
    // These are "active low" buttons, so we invert the values.
    int new_less_button_value = !gpio_get(LESS_BRIGHT_GPIO_PIN);
    int new_more_button_value = !gpio_get(MORE_BRIGHT_GPIO_PIN);

    bool less_button_changed = new_less_button_value != less_button_value;
    bool more_button_changed = new_more_button_value != more_button_value;

    less_button_value = new_less_button_value;
    more_button_value = new_more_button_value;
    button_held = less_button_value == 1 || more_button_value == 1;

    // Decrease brightness
    if (less_button_changed &&  less_button_value > 0 && dot_intensity <= (MAX_DOT_INTENSITY - DOT_INTENSITY_INCREMENT)) {
        dot_intensity += DOT_INTENSITY_INCREMENT;
    }
    else if (more_button_changed && more_button_value > 0 && (dot_intensity - DOT_INTENSITY_INCREMENT) >= MIN_DOT_INTENSITY) {        
        dot_intensity -= DOT_INTENSITY_INCREMENT;
    }
}

void fill_band_values (int max_intensity, int index, int values[]) {
    if (max_intensity) {
        values[index] += max_intensity;

        int bands = ((BAND_WIDTH -1) / 2);

        for (int a = 1; a <= bands; a++) {
            int band_intensity = (max_intensity / (a + 1));

            int leading_band_index = (index + a) % NUMPIXELS;
            values[leading_band_index] += band_intensity;

            int trailing_band_index = (index - a + NUMPIXELS) % NUMPIXELS;
            values[trailing_band_index] += band_intensity;
        }
    }
}
 

void increment_and_redraw_all_dots () {
    pixels.clear();

    TARGET_COLOUR target = COLOUR_WHEEL[target_colour_index];

    if (ticks % ROTATION_TICKS == 0) {
        positive_dot_index = (positive_dot_index + 1) % NUMPIXELS;
        negative_dot_index = (negative_dot_index + 1) % NUMPIXELS;
    }

    if (r == target.r && g == target.g && b == target.b) {
        target_colour_index = (target_colour_index + 1) % 6;
        target = COLOUR_WHEEL[target_colour_index];
    }

    if (r < target.r) {
        r++;
    }
    else if (r > target.r) {
        r--;
    }

    if (g < target.g) {
        g++;
    }
    else if (g > target.g) {
        g--;
    }

    if (b < target.b) {
        b++;
    }
    else if (b > target.b) {
        b--;
    }

    int adjusted_positive_r = (r * dot_intensity) / 255;
    int adjusted_positive_g = (g * dot_intensity) / 255;
    int adjusted_positive_b = (b * dot_intensity) / 255;

    int adjusted_negative_r = ((255 - r) * dot_intensity) / 255;
    int adjusted_negative_g = ((255 - g) * dot_intensity) / 255;
    int adjusted_negative_b = ((255 - b) * dot_intensity) / 255;

    int r_values[NUMPIXELS] = { 0 };
    int g_values[NUMPIXELS] = { 0 };
    int b_values[NUMPIXELS] = { 0 };

    fill_band_values(adjusted_positive_r, positive_dot_index, r_values);
    fill_band_values(adjusted_negative_r, negative_dot_index, r_values);        

    fill_band_values(adjusted_positive_g, positive_dot_index, g_values);
    fill_band_values(adjusted_negative_g, negative_dot_index, g_values);

    fill_band_values(adjusted_positive_b, positive_dot_index, b_values);
    fill_band_values(adjusted_negative_b, negative_dot_index, b_values);

    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(r_values[i], g_values[i], b_values[i]));
    }

    pixels.show();

    ticks++;
}

void draw_brightness_level () {
    pixels.clear();
    // For example, 32 is 0, and (127 - 32) / ((255-32)/16) = 95 / (213/16) = 95/13.31 =  7.14
    int brightness_index = (dot_intensity - MIN_DOT_INTENSITY) / DOT_INTENSITY_INCREMENT;

    uint32_t adjustedWhite = pixels.Color(dot_intensity, dot_intensity, dot_intensity);

    for (int i = 0; i < NUMPIXELS; i++) {
        if (i <= brightness_index) {
            pixels.setPixelColor(i, adjustedWhite);
        }
    }
 
    pixels.show();
}
int main () {
    setup();

    while (keep_running) {
        poll_button_values();


        if (button_held) {
            draw_brightness_level();
        }
        else {
            increment_and_redraw_all_dots();
        }


        // Wait for the next cycle.
        sleep_ms(TICK_MS);
    }
}