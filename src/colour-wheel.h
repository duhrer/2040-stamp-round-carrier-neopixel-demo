typedef struct {
    int r;
    int g;
    int b;
} TARGET_COLOUR;

TARGET_COLOUR RED    = { .r = 255, .g = 0,   .b = 0};
TARGET_COLOUR ORANGE = { .r = 255, .g = 165, .b = 0};
TARGET_COLOUR YELLOW = { .r = 255, .g = 255, .b = 0};
TARGET_COLOUR GREEN  = { .r = 0,   .g = 255, .b = 0};
TARGET_COLOUR BLUE   = { .r = 0,   .g = 0,   .b = 255};
TARGET_COLOUR PURPLE = { .r = 255, .g = 0,   .b = 255};

TARGET_COLOUR COLOUR_WHEEL[6] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
