
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void scrollText(const String& message) {
    int textWidth = message.length() * (8.5); // Calculate the width of the text in pixels, including space between characters
    int xPos = SCREEN_WIDTH; // Starting position of the text off the screen

    while (xPos > -textWidth) {
        display.clearDisplay(); // Clear the display buffer
        display.setCursor(xPos, 0); // Set the cursor position
        display.println(message); // Print the message
        display.display(); // Update the display

        xPos--; // Move the text to the left
    }
}

void setup() {
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

    scrollText("Good Luck! You'll need it when youre");
    
}

void loop() {
    
}

