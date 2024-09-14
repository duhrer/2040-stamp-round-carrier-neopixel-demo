#include <math.h>
#include <string.h>
#include <time.h>

#include "Adafruit_NeoPixel.hpp"


// See: https://lectronz.com/products/rp2040-stamp-round-carrier
#define PIN 24
#define NUMPIXELS 16

bool keep_running = true;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup () {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

# define MIN_TICKS 1
# define MAX_TICKS 50
# define TICK_MS 10

# define DOT_INTENSITY 128
# define BAND_WIDTH 11

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
    values[index] = DOT_INTENSITY;

    int bands = ((BAND_WIDTH -1) / 2);

    for (int a = 1; a <= bands; a++) {
        int band_intensity = (DOT_INTENSITY / (a + 1));
        int leading_band_index = (index + a) % NUMPIXELS;
        values[leading_band_index] = band_intensity;

        int trailing_band_index = (index - a + NUMPIXELS) % NUMPIXELS;
        values[trailing_band_index] = band_intensity;
    }
}

void increment_all () {
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

int main () {
    setup();

    while (keep_running) {
        increment_all();

        // Wait for the next cycle.
         sleep_ms(TICK_MS);
    }
}