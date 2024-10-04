#include <math.h>
#include <string.h>
#include <time.h>

#include <stdio.h>

#include "Adafruit_NeoPixel.hpp"


// See: https://lectronz.com/products/rp2040-stamp-round-carrier
#define PIN 24
#define NUMPIXELS 16

#define LESS_BRIGHT_GPIO_PIN 14
#define MORE_BRIGHT_GPIO_PIN 15

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

# define MIN_TICKS 1
# define MAX_TICKS 50
# define TICK_MS 10

# define MAX_DOT_INTENSITY 255
# define MIN_DOT_INTENSITY 32
# define DOT_INTENSITY_INCREMENT ((MAX_DOT_INTENSITY - MIN_DOT_INTENSITY)/NUMPIXELS)

# define BAND_WIDTH 11

uint less_button_value = 0;
uint more_button_value = 0;
bool button_held = false;

int dot_intensity = 128;

int TICK = 0;

int R_INDEX = 0;
int R_INDEX_DIRECTION = 1;
int R_TICKS = 25;

int G_INDEX = 4;
int G_INDEX_DIRECTION = 1;
int G_TICKS = 20;

int B_INDEX = 8;
int B_INDEX_DIRECTION = 1;
int B_TICKS = 10;

void fill_band_values (int index, int values[]) {
    values[index] = dot_intensity;

    int bands = ((BAND_WIDTH -1) / 2);

    for (int a = 1; a <= bands; a++) {
        int band_intensity = (dot_intensity / (a + 1));
        int leading_band_index = (index + a) % NUMPIXELS;
        values[leading_band_index] = band_intensity;

        int trailing_band_index = (index - a + NUMPIXELS) % NUMPIXELS;
        values[trailing_band_index] = band_intensity;
    }
}

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
    int TICK_OFFSET = TICK % MAX_TICKS;
    bool IS_DIRTY = false;

    if (TICK_OFFSET % R_TICKS == 0) {
        IS_DIRTY = true;
        int RAW_R_INDEX = R_INDEX + R_INDEX_DIRECTION;
        R_INDEX = (RAW_R_INDEX + NUMPIXELS) % NUMPIXELS;
    }

    if (TICK_OFFSET % G_TICKS == 0) {
        IS_DIRTY = true;
        int RAW_G_INDEX = G_INDEX + G_INDEX_DIRECTION;
        G_INDEX = (RAW_G_INDEX + NUMPIXELS) % NUMPIXELS;
    }

    if (TICK_OFFSET % B_TICKS == 0) {
        IS_DIRTY = true;
        int RAW_B_INDEX = B_INDEX + B_INDEX_DIRECTION;
        B_INDEX = (RAW_B_INDEX + NUMPIXELS) % NUMPIXELS;
    }

    if (IS_DIRTY) {
        pixels.clear();

        int R_VALUES[NUMPIXELS] = { 0 };
        fill_band_values(R_INDEX, R_VALUES);

        int G_VALUES[NUMPIXELS] = { 0 };
        fill_band_values(G_INDEX, G_VALUES);

        int B_VALUES[NUMPIXELS] = { 0 };
        fill_band_values(B_INDEX, B_VALUES);

        for (int i = 0; i < NUMPIXELS; i++) {
            int R_VAL = R_VALUES[i];
            int G_VAL = G_VALUES[i];
            int B_VAL = B_VALUES[i];

            if (R_VAL + G_VAL + B_VAL > 0) {
                pixels.setPixelColor(i, pixels.Color(R_VAL, G_VAL, B_VAL));
            }
        }

        pixels.show();
    }

    TICK++;
}

void draw_brightness_level () {
    pixels.clear();
    // For example, 32 is 0, and (127 - 32) / ((255-32)/16) = 95 / (213/16) = 95/13.31 =  7.14
    int brightness_index = (dot_intensity - MIN_DOT_INTENSITY) / DOT_INTENSITY_INCREMENT;
    for (int i = 0; i < NUMPIXELS; i++) {
        if (i <= brightness_index) {
            pixels.setPixelColor(i, pixels.Color(dot_intensity, dot_intensity, dot_intensity));
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