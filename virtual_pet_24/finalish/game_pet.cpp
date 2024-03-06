//you get the amount fo happiness points as there are time left on the timer in seconds.
#include "game_pet.h"
int BOX_SIZE = 8; // Size of each box in pixels
int cursorX, cursorY = 0;
int petDinoX = 64;
int petDinoY = 32;
int lastTime = -1;
const int cursorSize = 8;
const int dinoSize = 16;
static int remainingTime = 30;  // Initial time in seconds
int count = 0;
bool seen[5][5];
//for timer 
unsigned long petPreviousMillis = 0;
const unsigned long interval = 1000;


void startPetGame(Adafruit_SH1106G &display) {
  display.clearDisplay();
  remainingTime = 30;
  count = 0;
  Serial.println("in pet game");
  display.drawRect(0, 16, 32, 32, SH110X_WHITE);
  display.drawBitmap(16, 0, bitmap_right_tail_down, 128, 64, SH110X_WHITE);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      seen[i][j] = false; // Initially, no box is hovered over or seen
    }
  }
  drawWhiteBoxes(display); // Draw a grid of white boxes
  delay(100); // Delay for stability
  display.display();
}

void updatePetGame(Adafruit_SH1106G &display, int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY){
  //following code is to display timer
  unsigned long currentMillis = millis();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Time Remaining: ");
  display.print(remainingTime);
  Serial.println(count);

  if (currentMillis - petPreviousMillis >= interval) {
    petPreviousMillis = currentMillis;
    if (remainingTime >= 0) {
      remainingTime--;
    } 
    if(count >= 25 || remainingTime < 0){
      HAPPINESS += remainingTime;
      Serial.println(HAPPINESS);
      selected[1] = false;
      isGameActive = false;
      //game end code since no more remaining time user just has to press the main putton
    }
  }

  if (joyX > 100) cursorX -= 5;
  if (joyX < 28) cursorX += 5;
  if (joyY > 70) cursorY -= 5;
  if (joyY < 16) cursorY += 5;

  // Constrain cursor position to display boundaries
  cursorX = constrain(cursorX, 0, SCREEN_WIDTH - cursorSize);
  cursorY = constrain(cursorY, 0, SCREEN_HEIGHT - cursorSize);
  display.drawBitmap(cursorX, cursorY, bitmap_hand_cursor, 16, 16, SH110X_WHITE);

  // Check if the cursor hovers over any of the boxes
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      int boxX = (i * BOX_SIZE)+48;
      int boxY = (j * BOX_SIZE)+20;
      if (cursorX >= boxX && cursorX < boxX + BOX_SIZE && cursorY >= boxY && cursorY < boxY + BOX_SIZE) {
        // If the cursor is over the box, clear it
        display.fillRect(boxX, boxY, BOX_SIZE, BOX_SIZE, SH110X_BLACK);
        if(!seen[i][j]) {
          count++;
        }
        seen[i][j] = true;
        break; // Exit the loop after clearing one box
      }
    }
  }
  drawWhiteBoxes(display); // Draw a grid of white boxes
  delay(20);
  display.drawBitmap(16, 0, bitmap_right_tail_down, 128, 64, SH110X_WHITE);

}

void drawWhiteBoxes(Adafruit_SH1106G &display) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (seen[i][j] == false){
        display.drawRect((i * BOX_SIZE)+48, (j * BOX_SIZE)+20, BOX_SIZE, BOX_SIZE, SH110X_WHITE);
        display.fillRect((i * BOX_SIZE)+48, (j * BOX_SIZE)+20, BOX_SIZE, BOX_SIZE, SH110X_WHITE);
      }
    }
  }
}
