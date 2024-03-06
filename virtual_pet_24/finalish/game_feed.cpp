#include "game_feed.h"

// Definitions of global variables
int foodY, foodX;
const int foodSize = 4; // Size of food bits
int foodFallSpeed = 3;
int feedDinoX = 0;
bool pointLeft = true;

// Function definitions
void startFeedGame(int SCREEN_WIDTH) {
    Serial.println("in feed game");
    foodY = 0;
    foodX = random(0, SCREEN_WIDTH - foodSize);
}

void updateFeedGame(Adafruit_SH1106G &display, int joyX, int joyButton, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
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

    if (foodY >= 40 && foodY <= 60 && foodX >= feedDinoX && foodX <= feedDinoX + 20 && NUTRITION <= 98) {
        NUTRITION += 2;
        foodY = 0;
        foodX = random(0, SCREEN_WIDTH - foodSize);
    }

    if(NUTRITION >= 100) {
        selected[0] = false;
        isGameActive = false;
    }
}

void drawGame(Adafruit_SH1106G &display) {
  // Draw dinosaur
  if(pointLeft) {
    display.drawBitmap(feedDinoX, 44, bitmap_small_dino_left, DINO_WIDTH, DINO_HEIGHT, SH110X_WHITE);
  }else{
    display.drawBitmap(feedDinoX, 44, bitmap_small_dino_right, DINO_WIDTH, DINO_HEIGHT, SH110X_WHITE);
  }
  
  // Draw food
  display.fillRect(foodX, foodY, foodSize, foodSize, SH110X_WHITE);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("NUTRITION: ");
  display.print(NUTRITION);
}