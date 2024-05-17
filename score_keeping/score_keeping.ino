/*
Copyright (c) 2024 Samuel Asebrook, Erik Johnson, Mika Burmeister, and Ryan Campbell
*/

#include <ezButton.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

#define buzzerPin 7
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize the WS2812B LED strip with fastLED
#define NUM_LEDS 60
#define LED_PIN 16
CRGB leds[NUM_LEDS];

// Piezo Pin: 17




// Define the pin numbers for the sensors
const int opticalSensorPin = A0; // 5V
const int limitSwitchPin = 2; // 3.3V

unsigned long startTime;
unsigned long elapsedTime;
int multiplier = 1;
int score = 0;
int highScore = 0;
bool gameStarted = false;
bool state1_moved = false;
bool state2_moved = false;
bool state3_moved = false;
bool state4_moved = false;

int led_1_1 = 22;
int led_1_2 = 23;
int led_1_3 = 24;
int led_1_4 = 25;

int led_2_1 = 40;
int led_2_2 = 41;
int led_2_3 = 42;
int led_2_4 = 43;

int led_3_1 = 34;
int led_3_2 = 35;
int led_3_3 = 36;
int led_3_4 = 37;

int led_4_1 = 28;
int led_4_2 = 29;
int led_4_3 = 30;
int led_4_4 = 31;


int state_1, state_2, state_3, state_4 = 0; // Dummy variables strictly for local compilation purposes, REMOVE UPON GROUP COMPILATION
ezButton sw1(13);
ezButton sw2(18);
ezButton sw3(19);
ezButton sw4(11);
ezButton startSw(10);
ezButton stopSw(15);

void hitSound() {
  // Play a sound when the launcher hits the limit switch
  tone(buzzerPin, 1000, 100);
}

void hitLEDStrip() {
  // Flash the LED strip when the launcher hits the limit switch
  fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(100);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
}

void gameStartLEDStrip() {
  // Do a blue snake animation on the LED strip when the game starts
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(50);
  }
}

void gameStartSong() {
  // Play a song when the game starts
  tone(buzzerPin, 1000, 100);
  delay(100);
  tone(buzzerPin, 1500, 100);
  delay(100);
  tone(buzzerPin, 2000, 100);
}

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
    display.clearDisplay(); // Clear the display buffer
}


void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
    display.display();  
    delay(1);
    display.clearDisplay();
    display.setTextWrap(false);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    scrollText("Welcome to Battleship!");

  // Init LEDs 
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    // leds fill rainbow
    fill_rainbow(leds, NUM_LEDS, 0, 255);
    FastLED.show();
    gameStartLEDStrip();
    gameStartSong();

  

  pinMode(opticalSensorPin, INPUT);
  pinMode(limitSwitchPin, INPUT_PULLUP);

  sw1.setDebounceTime(50);
  sw2.setDebounceTime(50);
  sw3.setDebounceTime(50);
  sw4.setDebounceTime(50);
  startSw.setDebounceTime(50);
  stopSw.setDebounceTime(50);

  pinMode(led_1_1, OUTPUT);
  pinMode(led_1_2, OUTPUT);
  pinMode(led_1_3, OUTPUT);
  pinMode(led_1_4, OUTPUT);
  
  pinMode(led_2_1, OUTPUT);
  pinMode(led_2_2, OUTPUT);
  pinMode(led_2_3, OUTPUT);
  pinMode(led_2_4, OUTPUT);

  pinMode(led_3_1, OUTPUT);
  pinMode(led_3_2, OUTPUT);
  pinMode(led_3_3, OUTPUT);
  pinMode(led_3_4, OUTPUT);

  pinMode(led_4_1, OUTPUT);
  pinMode(led_4_2, OUTPUT);
  pinMode(led_4_3, OUTPUT);
  pinMode(led_4_4, OUTPUT);
}

void loop() {

  //int opticalSensorState = digitalRead(opticalSensorPin);
  //int limitSwitchState = digitalRead(limitSwitchPin);

  sw1.loop();
  sw2.loop();
  sw3.loop();
  sw4.loop();
  startSw.loop();
  stopSw.loop();  

  if(sw1.isPressed()) {
   state_1++;
   hitLEDStrip();
   hitSound();
  }

  if(sw2.isPressed()) {
   state_2++;
   hitLEDStrip();
   hitSound();
  }

  if(sw3.isPressed()) {
   state_3++;
   hitLEDStrip();
   hitSound();
  }

  if(sw4.isPressed()) {
   state_4++;
   hitLEDStrip();
   hitSound();
  }


  if (startSw.isPressed() && !gameStarted) {
    // The launcher has been pulled back, start the game
    startTime = millis();
    gameStarted = true;

    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0,0);
    // display.println("Good Luck!");
    // display.setCursor(0,16);
    // display.println("You Will Need It!");
    // display.display();
    // delay(1000);
  }

  if ((state_1 == 1 && state1_moved == false) || (state_2 == 1 && state2_moved == false) || (state_3 == 1 && state3_moved == false) || (state_4 == 1 && state4_moved == false) ) {
    score = score + (multiplier * 100);

    if (state_1 == 1) {
      state1_moved = true;
      digitalWrite(led_1_1, HIGH);
    } else if (state_2 == 1) {
      state2_moved = true;
      digitalWrite(led_2_1, HIGH);
    } else if (state_3 == 1) {
      state3_moved = true;
      digitalWrite(led_3_1, HIGH);
    } else if (state_4 == 1) {
      state4_moved = true;
      digitalWrite(led_4_1, HIGH);
    }
  }

  if ((state_1 == 2 && state1_moved == true) || (state_2 == 2 && state2_moved == true) || (state_3 == 2 && state3_moved == true) || (state_4 == 2 && state4_moved == true) ) {
    score = score + (multiplier * 200);

    if (state_1 == 2) {
      state1_moved = false;
      digitalWrite(led_1_2, HIGH);
    } else if (state_2 == 2) {
      state2_moved = false;
      digitalWrite(led_2_2, HIGH);
    } else if (state_3 == 2) {
      state3_moved = false;
      digitalWrite(led_3_2, HIGH);
    } else if (state_4 == 2) {
      state4_moved = false;
      digitalWrite(led_4_2, HIGH);
    }
  }

  if ((state_1 == 3 && state1_moved == false) || (state_2 == 3 && state2_moved == false) || (state_3 == 3 && state3_moved == false) || (state_4 == 3 && state4_moved == false) ) {
    score = score + (multiplier * 300);

    if (state_1 == 3) {
      state1_moved = true;
      digitalWrite(led_1_3, HIGH);
    } else if (state_2 == 3) {
      state2_moved = true;
      digitalWrite(led_2_3, HIGH);
    } else if (state_3 == 3) {
      state3_moved = true;
      digitalWrite(led_3_3, HIGH);
    } else if (state_4 == 3) {
      state4_moved = true;
      digitalWrite(led_4_3, HIGH);
    }
  }

  if ((state_1 == 4 && state1_moved == true) || (state_2 == 4 && state2_moved == true) || (state_3 == 4 && state3_moved == true) || (state_4 == 4 && state4_moved == true) ) {
    score = score + (multiplier * 1000);

    if (state_1 == 4) {
      state1_moved = false;
      digitalWrite(led_1_4, HIGH);
    } else if (state_2 == 4) {
      state2_moved = false;
      digitalWrite(led_2_4, HIGH);
    } else if (state_3 == 4) {
      state3_moved = false;
      digitalWrite(led_3_4, HIGH);
    } else if (state_4 == 4) {
      state4_moved = false;
      digitalWrite(led_4_4, HIGH);
    }
  }

  if (state_1 >= 4 && state_2 >= 4 && state_3 >= 4 && state_4 >= 4) {
    state_1 = 0;
    state_2 = 0;
    state_3 = 0;
    state_4 = 0;

    digitalWrite(led_1_1, LOW);
    digitalWrite(led_1_2, LOW);
    digitalWrite(led_1_3, LOW);
    digitalWrite(led_1_4, LOW);

    digitalWrite(led_2_1, LOW);
    digitalWrite(led_2_2, LOW);
    digitalWrite(led_2_3, LOW);
    digitalWrite(led_2_4, LOW);

    digitalWrite(led_3_1, LOW);
    digitalWrite(led_3_2, LOW);
    digitalWrite(led_3_3, LOW);
    digitalWrite(led_3_4, LOW);

    digitalWrite(led_4_1, LOW);
    digitalWrite(led_4_2, LOW);
    digitalWrite(led_4_3, LOW);
    digitalWrite(led_4_4, LOW);


    multiplier = multiplier * 2;
  }

  if (gameStarted && stopSw.isPressed()) {
    // The limit switch has been pressed, end the game
    elapsedTime = millis() - startTime;
    gameStarted = false;

    // Calculate the score
    score = score + (elapsedTime / 10);
    if (score > highScore) {
      highScore = score;
    }
  state_1 = 0;
  state_2 = 0;
  state_3 = 0;
  state_4 = 0;

  // Display the score on the OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Score: ");
  display.println(score);

  display.setCursor(0,16);
  display.print("High: ");
  display.println(highScore);

  display.display();

  score = 0;
  multiplier = 1;
}
}
