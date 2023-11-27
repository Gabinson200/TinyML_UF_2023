#include "game_feed.h"

// Definitions of global variables
bool isGameActive = false;
unsigned long gameStartTime;
const unsigned long gameDuration = 30000; // 30 seconds
int foodY, foodX;
const int foodSize = 4; // Size of food bits
int foodFallSpeed = 2;
int feedDinoX = 0;
bool pointLeft = true;

// Function definitions
void startFeedGame(int SCREEN_WIDTH) {
    gameStartTime = millis();
    foodY = 0;
    foodX = random(0, SCREEN_WIDTH - foodSize);
}

void updateFeedGame(Adafruit_SSD1306 &display, int joyX, int joyButton, int hunger, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    display.clearDisplay();
    drawGame(display);

    // Your code for moving the dino, handling food movement, collision, and game end
    if(joyX < 28){
        feedDinoX += 5;
        pointLeft = false;
        if(feedDinoX > 108){
            feedDinoX = 108;
        }
    } else if(joyX > 100){
        feedDinoX -= 5;
        pointLeft = true;
        if(feedDinoX < 0){
            feedDinoX = 0;
        }
    }

    foodY += foodFallSpeed;
    if (foodY > SCREEN_HEIGHT) {
        foodY = 0;
        foodX = random(0, SCREEN_WIDTH - foodSize);
    }

    if (foodY >= 40 && foodY <= 60 && foodX >= feedDinoX && foodX <= feedDinoX + 20) {
        hunger += 5;
        foodY = 0;
        foodX = random(0, SCREEN_WIDTH - foodSize);
    }

    if (millis() - gameStartTime > gameDuration) {
        isGameActive = false;
        // End of game logic
    }

}

void drawGame(Adafruit_SSD1306 &display) {
  // Draw dinosaur
  if(pointLeft) {
    display.drawBitmap(feedDinoX, 44, bitmap_small_dino_left, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
  }else{
    display.drawBitmap(feedDinoX, 44, bitmap_small_dino_right, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
  }
  
  // Draw food
  display.fillRect(foodX, foodY, foodSize, foodSize, SSD1306_WHITE);
}