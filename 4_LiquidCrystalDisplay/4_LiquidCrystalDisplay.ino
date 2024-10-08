//************************************************************
// Name: Matthew Eagan & Liam O'Brien
// Course: CS328 - Embedded Systems
// Assignment: Lab 4 - Liquid Crystal Display
// Purpose: To utilize a Bluetooth connection to display
//          animated sprites on a liquid crystal display
// Due Date: 10-10-2024
//************************************************************

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define BLUETOOTH_BAUD_RATE 9600
#define Rx 2
#define Tx 3

//Dsignate Rx and Tx pins for Bluetooth communications
SoftwareSerial bluetooth(Rx, Tx);
int bluetoothData;

// Initialize the LCD at I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define LED pins
const int ledLeftArrow = 9;
const int ledRightArrow = 10;

// Custom character bitmaps (5x8 pixel format)
byte leftArrow[8] = {
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b01100,
  0b00100,
  0b00000,
  0b00000
};

byte rightArrow[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000,
  0b00000
};

byte customSprite[8] = {
  0b00000,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

void setup() {
  //Initialize Bluetooth pins for input and output
  pinMode(Rx, INPUT);
  pinMode(Tx, OUTPUT);

  //Start Bluetooth receiver
  bluetooth.begin(BLUETOOTH_BAUD_RATE);
  
  // Set up the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  lcd.backlight(); // Turn on the backlight

  // Create custom characters
  lcd.createChar(0, leftArrow);
  lcd.createChar(1, rightArrow);
  lcd.createChar(2, customSprite);

  // Set LED pins as output
  pinMode(ledLeftArrow, OUTPUT);
  pinMode(ledRightArrow, OUTPUT);

  // Initial display
  lcd.setCursor(0, 0);
  lcd.write(byte(2)); // Display custom sprite initially
}

String readCommand() {
  String asciiData = "";

  if (bluetooth.available()) {
    lcd.clear();

    while (bluetooth.available()) {
      bluetoothData = bluetooth.read();

      if (bluetooth != 13 && bluetoothData != 10) {
        asciiData += (char)bluetoothData;
      }
    }
  }

  return asciiData;
}

void loop() {
  // Randomly decide which LED and arrow to flash
  int randomLED = random(0, 3); // 0 = none, 1 = left, 2 = right

  if (randomLED == 1) {
    // Flash left LED and left arrow sprite
    digitalWrite(ledLeftArrow, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0)); // Display left arrow
    delay(500);
    digitalWrite(ledLeftArrow, LOW);
  } else if (randomLED == 2) {
    // Flash right LED and right arrow sprite
    digitalWrite(ledRightArrow, HIGH);
    lcd.clear();
    lcd.setCursor(15, 0);
    lcd.write(byte(1)); // Display right arrow
    delay(500);
    digitalWrite(ledRightArrow, LOW);
  } else {
    // No LED, display custom sprite
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.write(byte(2)); // Display custom sprite
    delay(500);
  }

  delay(500); // Wait before next cycle
}