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

# define TICK_MS 10

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

void increment_and_redraw_all_dots () {
    pixels.clear();

    TARGET_COLOUR target = COLOUR_WHEEL[target_colour_index];

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

    int adjusted_r = (r * dot_intensity) / 255;
    int adjusted_g = (g * dot_intensity) / 255;
    int adjusted_b = (b * dot_intensity) / 255;

    uint32_t pixelColor = pixels.Color(adjusted_r, adjusted_g, adjusted_b);

    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixelColor);
    }

    pixels.show();
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