/*
Copyright (c) 2024 Samuel Asebrook
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ezButton.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
// LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// Define the pin numbers for the sensors
const int opticalSensorPin = A0; // 5V
const int limitSwitchPin = 2; // 3.3V
// LCD SDA Pin = A4
// LCD SCL Pin = A5

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

int led_1_1 = 23;
int led_1_2 = 22;
int led_1_3 = 0;
int led_1_4 = 0;

int led_2_1 = 27;
int led_2_2 = 26;
int led_2_3 = 0;
int led_2_4 = 0;

int led_3_1 = 31;
int led_3_2 = 30;
int led_3_3 = 0;
int led_3_4 = 0;

int led_4_1 = 35;
int led_4_2 = 34;
int led_4_3 = 0;
int led_4_4 = 0;


int state_1, state_2, state_3, state_4 = 0; // Dummy variables strictly for local compilation purposes, REMOVE UPON GROUP COMPILATION
ezButton sw1(19);
ezButton sw2(18);
ezButton sw3(4);
ezButton sw4(3);

void scrollText(int row, String message, int delayTime) {
  message = message + "           ";

  for (int i = 0; i < message.length(); i++) {
    display.clearDisplay();
    display.setCursor(0, row);
    display.print(message.substring(i));
    display.display();

    delay(delayTime);
  }
}

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Commenting out lcd code in favor of OLED TODO: TRANSFER
  // lcd.init(); 
  // lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("Hell Yeah!");
  // lcd.setCursor(0, 1);
  // lcd.print("Pinball!");
  display.display(); // Show initial display buffer contents on the screen
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Good Luck!");
  display.setCursor(0,16);
  display.println("You Will Need It!");
  display.display();
  display.clearDisplay();
  // Display a long line of text and scroll through it using the adafuit
  display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0,0);
display.setTextWrap(false);  // Disable text wrapping

// Split your long text into multiple shorter strings
String longText = "This is a very long line of text that will need to scroll across the display.";
scrollText(0, longText, 100);
  

  pinMode(opticalSensorPin, INPUT);
  pinMode(limitSwitchPin, INPUT_PULLUP);

  sw1.setDebounceTime(50);
  sw2.setDebounceTime(50);
  sw3.setDebounceTime(50);
  sw4.setDebounceTime(50);

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
  int opticalSensorState = digitalRead(opticalSensorPin);
  int limitSwitchState = digitalRead(limitSwitchPin);

  sw1.loop();
  sw2.loop();
  sw3.loop();
  sw4.loop();

  if(sw1.isPressed()) {
   state_1++;
  }

  if(sw2.isPressed()) {
   state_2++;
  }

  if(sw3.isPressed()) {
   state_3++;
  }

  if(sw4.isPressed()) {
   state_4++;
  }


  if (opticalSensorState == HIGH && !gameStarted) {
    // The launcher has been pulled back, start the game
    startTime = millis();
    gameStarted = true;
    // TODO: TRANSFER
    // lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("Good Luck!");
    // lcd.setCursor(0, 1);
    // lcd.print("You Need It!");

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Good Luck!");
    display.setCursor(0,16);
    display.println("You Will Need It!");
    display.display();
    delay(1000);
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

  if (state_1 == 4 && state_2 == 4 && state_3 == 4 && state_4 == 4) {
    state_1 == 0;
    state_2 == 0;
    state_3 == 0;
    state_4 == 0;

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

  if (gameStarted && limitSwitchState == LOW) {
    // The limit switch has been pressed, end the game
    elapsedTime = millis() - startTime;
    gameStarted = false;

    // Calculate the score
    score = score + (elapsedTime / 10);
    if (score > highScore) {
      highScore = score;
    }

    // Display the score on the LCD TODO: TRANSFER
    // lcd.clear();
    display.clearDisplay();

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
