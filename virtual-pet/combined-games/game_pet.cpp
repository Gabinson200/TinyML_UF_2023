#include "game_pet.h"
int cursorX, cursorY = 0;
int petDinoX = 64;
int petDinoY = 32;
int lastTime = -1;

void startPetGame(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Coordinates and dimensions for the "Back" box
  int backX = 0, backY = 0;
  int backWidth = 20, backHeight = 12;  // Adjust size as needed

  // Draw a rectangle around "Back"
  display.drawRect(backX, backY, backWidth, backHeight, WHITE);

  // Draw "Back" text inside the rectangle
  display.setCursor(backX + 4, backY + 2); // Adjust cursor position as needed
  display.println("<-");

}

void updatePetGame(Adafruit_SSD1306 &display, int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY, int happiness){
  
  const int cursorSize = 16;
  const int dinoSize = 16;

  if (joyX > 64) cursorX -= 5;
  if (joyX < 64) cursorX += 5;
  if (joyY > 32) cursorY -= 5;
  if (joyY < 32) cursorY += 5;

  // Constrain cursor position to display boundaries
  cursorX = constrain(cursorX, 0, SCREEN_WIDTH - 16);
  cursorY = constrain(cursorY, 0, SCREEN_HEIGHT - 16);

  // Game display logic
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Draw the "Back" button
  //startPetGame(display);
  
  // Or replicate the "Back" button drawing code here
  // Draw the cursor
  display.drawBitmap(cursorX, cursorY, bitmap_hand_cursor, 16, 16, SSD1306_WHITE);

  unsigned long currentTime = millis();


  if (cursorX < petDinoX + dinoSize && 
      cursorX + cursorSize > petDinoX &&
      cursorY < petDinoY + dinoSize &&
      cursorY + cursorSize > petDinoY) {

    lastTime = currentTime;

    // Collision detected, reposition dino
    petDinoX = random(0, SCREEN_WIDTH - 16);
    petDinoY = random(0, SCREEN_HEIGHT - 16);
    happiness += 1;
  }else if ((currentTime - lastTime > 5000)) { // 5 seconds passed
    lastTime = currentTime;
    petDinoX = random(0, SCREEN_WIDTH - 16);
    petDinoY = random(0, SCREEN_HEIGHT - 16);
  }

  // Draw the small dino
  display.drawBitmap(petDinoX, petDinoY, bitmap_small_dino, 16, 16, SSD1306_WHITE);
  display.display();

}

