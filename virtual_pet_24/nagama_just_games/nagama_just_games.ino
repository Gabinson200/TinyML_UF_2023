// This sketch is formatted for the TinyML Club, focusing on comprehensive comments to keep the code clean and efficient.

// Include necessary libraries and files
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "dino_sprites.h"
#include "game_feed.h"
#include "game_play.h"
#include "game_pet.h"
#include "Globals.h"

// Define OLED screen parameters
#define i2c_Address 0x3C // Initialize with the I2C address 0x3C; typically for eBay OLEDs, adjust if using a different source
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define I2C_SDA 21 // Microcontroller pins to which the OLED is attached
#define I2C_SCL 22  

TwoWire I2C_OLED = TwoWire(0);
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, OLED_RESET);

// Define joystick pins
#define JOY_X 35 // Joystick X-axis analog pin
#define JOY_Y 32 // Joystick Y-axis analog pin
#define JOY_BTN 13 // Joystick button pin

// Constants for button dimensions and positions
const int buttonWidth = 32;
const int buttonHeight = 12;
const int buttonY = 0;
const int buttonX[4] = {0, 32, 64, 96};
//const char* menuLabels[4] = {"Feed", "Pet", "Nap", "Play"};
bool selected[4] = {false, false, false, false};

// Variables for joystick and game states
int joyX = 0;
int joyY = 0;
int joyButton = LOW;
int menuCount = 0;

// Dinosaur position and speed variables
int dinosaurX = 0;
int dinosaurY = 0;
int dinosaurSpeed = 2;

// Internal state values [0, 100] for hunger, happiness, and tiredness
int NUTRITION = 48;
int HAPPINESS = 48;
int ENERGY = 48;
bool isGameActive = false;

// Timer variables
unsigned long previousMillis = 0;

// Arrays holding dinosaur sprite images
unsigned char* dino_right[] = {bitmap_right_tail_up, bitmap_right_tail_down};
unsigned char* dino_left[] = {bitmap_left_tail_up, bitmap_left_tail_down};
unsigned char* current_dino_sprite;

void setup() {
  Serial.begin(115200);

  // Set joystick button pin as input with pull-up
  pinMode(JOY_BTN, INPUT_PULLUP);

  // Initialize OLED display
  I2C_OLED.begin(I2C_SDA, I2C_SCL, 100000); 
  display.begin(0x3C, true); // Address 0x3C default
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.println("Display configured");
}

void gameSelection() {
  if(selected[0]) {
    // Update Feed game
    updateFeedGame(display, joyX, joyButton, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (joyButton == LOW) {
      delay(100);
      selected[0] = false;
    }
  } else if(selected[1]) {
    // Update Pet game
    updatePetGame(display, SCREEN_WIDTH, SCREEN_HEIGHT, joyX, joyY, happiness);
    if (joyButton == LOW) {
      delay(100);
      selected[1] = false;
    }
  } else if(selected[3]) {
    // Update Play game
    updatePlayGame(display, SCREEN_WIDTH, SCREEN_HEIGHT, joyX, joyY, isGameActive);
    if (joyButton == LOW) {
      delay(100);
      selected[3] = false;
    }
  }
}

void menuNavigation() {
  if(joyX > 100) {
    menuCount--;
    if(menuCount < 0) {
      menuCount = 3;
    }
  } else if(joyX < 28) {
    menuCount++;
    if(menuCount > 3) {
      menuCount = 0;
    }
  }
}

//Flash the selected button by toggling between black and white
void flashButtons() {
    for (int i = 0; i < 4; i++) {
    if (i == menuCount) {
      // Flash the selected button by toggling between black and white
      static bool flashState = false;
      display.drawRect(buttonX[i], buttonY, buttonWidth, buttonHeight, flashState ? SH110X_BLACK : SH110X_WHITE);
      flashState = !flashState;
      delay(20);
    } else {
      display.drawRect(buttonX[i], buttonY, buttonWidth, buttonHeight, SH110X_WHITE);
      delay(20);
    }
  }
}

void displayDinosaur() {
      // Get dinosaur sprite to draw based on happiness and movement direction
    if(dinosaurSpeed >= 0) {
      current_dino_sprite = dino_right[happiness > 50 ? 0 : 1];
    } else {
      current_dino_sprite = dino_left[happiness > 50 ? 0 : 1];
    }

    display.drawBitmap(dinosaurX, dinosaurY, current_dino_sprite, 128, 64, SH110X_WHITE);

    // Move the dinosaur horizontally
    dinosaurX += dinosaurSpeed;

    // Check for boundaries and reverse direction
    if (dinosaurX <= -64 || dinosaurX >= SCREEN_WIDTH - 64) {
      dinosaurSpeed = -dinosaurSpeed;
    }  
}

void checkForEnterGame() {
      // Example of triggering the game
    if (menuCount == 0 && joyButton == LOW && selected[0] == false) { // Assuming "Feed" is the first menu item
      delay(70);
      startFeedGame(SCREEN_WIDTH);
      selected[0] = true;
      isGameActive = true;
    }

    if (menuCount == 3 && joyButton == LOW && selected[3] == false) { // Assuming "Play" is the fourth menu item
      delay(70);
      startPlayGame(SCREEN_WIDTH, SCREEN_HEIGHT);
      selected[3] = true;
      isGameActive = true;
    }

    if (menuCount == 1 && joyButton == LOW && selected[1] == false ) { // Assuming "Pet" is the second menu item
      delay(70);
      startPetGame(display);
      selected[1] = true;
      isGameActive = true;
    }
}

void loop() {
  // Read and map joystick input values, these may change depending on the joystick you use!!!
  joyX = map(analogRead(JOY_X), 0, 1023, 0, 32);
  joyY = map(analogRead(JOY_Y), 0, 1023, 0, 32);
  joyButton = digitalRead(JOY_BTN);
  display.clearDisplay();


  // Check for each game, if one is active, run the code for it
  if(isGameActive) {
    gameSelection();
  }
  // Game is not in progress
  else {
    menuNavigation();
    flashButtons();
    displayDinosaur();
    displayMenuText();
    checkForEnterGame();
  }

  display.display(); // Read from display buffer
  delay(20); // Speed up simulation
}

// Function to display menu text
void displayMenuText() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(4, 3);
  display.println("Feed");

  display.setCursor(40, 3);
  display.println("Pet");

  display.setCursor(72, 3);
  display.println("Nap");

  display.setCursor(101, 3);
  display.println("Play");
}
