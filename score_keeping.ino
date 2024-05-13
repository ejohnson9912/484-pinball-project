/*
Copyright (c) 2024 Samuel Asebrook
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// Define the pin numbers for the sensors
const int opticalSensorPin = A0; // 5V
const int limitSwitchPin = 2; // 3.3V
// LCD SDA Pin = A4
// LCD SCL Pin = A5

unsigned long startTime;
unsigned long elapsedTime;
int highScore = 0;
bool gameStarted = false;

void scrollText(int row, String message, int delayTime) {
  message = message + "           ";

  for (int i = 0; i < message.length(); i++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(i));

    delay(delayTime);
  }
}

void setup() {
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hell Yeah!");
  lcd.setCursor(0, 1);
  lcd.print("Pinball!");

  pinMode(opticalSensorPin, INPUT);
  pinMode(limitSwitchPin, INPUT_PULLUP);
}

void loop() {
  int opticalSensorState = digitalRead(opticalSensorPin);
  int limitSwitchState = digitalRead(limitSwitchPin);

  if (opticalSensorState == HIGH && !gameStarted) {
    // The launcher has been pulled back, start the game
    startTime = millis();
    gameStarted = true;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Good Luck!");
    lcd.setCursor(0, 1);
    lcd.print("You Need It!");
  }

  if (gameStarted && limitSwitchState == LOW) {
    // The limit switch has been pressed, end the game
    elapsedTime = millis() - startTime;
    gameStarted = false;

    // Calculate the score
    int score = elapsedTime / 100; // Divide by 100; 1 second = 10 points
    if (score > highScore) {
      highScore = score;
    }

    // Display the score on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.print(score);

    if (String(highScore).length() >= 11) {
      scrollText(1, " High: " + String(highScore), 500);
    }
    lcd.setCursor(0, 1);
    lcd.print("High: ");
    lcd.print(highScore);
  }
}
