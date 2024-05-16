#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(100);
}

void loop() {
    static uint8_t snakeSize = 5;
    static uint8_t snakeSpeed = 5;
    static int headPos = 0;

    // Move the snake by shifting LEDs
    for (int i = NUM_LEDS - 1; i > 0; i--) {
        leds[i] = leds[i - 1];
    }

    // Set the color of the head of the snake
    leds[headPos] = CRGB::Blue;

    // Clear the tail of the snake
    int tailPos = headPos - snakeSize;
    if (tailPos < 0) {
        tailPos += NUM_LEDS;
    }
    leds[tailPos] = CRGB::Black;

    // Update head position
    headPos++;
    if (headPos >= NUM_LEDS) {
        headPos = 0;
    }

    FastLED.show();
    FastLED.delay(1000 / snakeSpeed);
}
