#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LED_PIN 6
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

void scrollText(const String& message) {
    int textWidth = message.length() * (8.5); // Calculate the width of the text in pixels, including space between characters
    int xPos = SCREEN_WIDTH; // Starting position of the text off the screen

    while (xPos > -textWidth) {
        display.clearDisplay(); // Clear the display buffer
        display.setCursor(xPos, 0); // Set the cursor position
        display.println(message); // Print the message
        display.display(); // Update the display

        xPos--; // Move the text to the left
        delay(20); // Add a small delay to control the scrolling speed
    }
}

void setup() {
    // Initialize the OLED display
    Serial.begin(9600);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextWrap(false);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    // Initialize the LED strip
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(100);

    // Start the scrolling text
    
}

void loop() {
    scrollText("Good Luck! You'll need it when you're");
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
