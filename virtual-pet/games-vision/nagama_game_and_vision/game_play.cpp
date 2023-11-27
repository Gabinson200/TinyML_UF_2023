#include "game_play.h"
#include "game_feed.h"

const unsigned long happinessIncreaseInterval = 5000; // 5 seconds
int lives = 3;
const int asteroidCount = 5;
int asteroidsX[asteroidCount];
int asteroidsY[asteroidCount];
int asteroidsSpeed[asteroidCount];
int playDinoY = 32;
int playDinoX = 64;

void startPlayGame(int screenWidth, int screenHeight) {
  gameStartTime = millis();
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] = random(screenWidth); // Random X position
    asteroidsY[i] = random(screenHeight); // Random Y position
    asteroidsSpeed[i] = random(1, 4); // Random speed
  }
}

void updatePlayGame(Adafruit_SSD1306 &display,int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY) {
  display.clearDisplay();

  display.drawBitmap(playDinoX, playDinoY, bitmap_small_dino, 20, 20, SSD1306_WHITE);

  //changing and drawing asteroids
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] -= asteroidsSpeed[i]; // Move leftward
    if (asteroidsX[i] < 0) {
      asteroidsX[i] = SCREEN_WIDTH;
      asteroidsY[i] = random(SCREEN_HEIGHT); // New random Y position
    }
    display.fillCircle(asteroidsX[i], asteroidsY[i], 3, SSD1306_WHITE); // Draw asteroid
  }

  // Update dinosaur position based on joystick input
  if (joyX < 28) {
    playDinoX += 5; // Move right
    if(playDinoX > SCREEN_WIDTH - 16) {
      playDinoX = SCREEN_WIDTH - 16;
    }
  }else if (joyX > 100) {
    playDinoX -= 5; // Move left
    if (playDinoX < 0) {
      playDinoX = 0;
    }
  }

  if (joyY < 28) {
    playDinoY += 5; // Move down
    if(playDinoY > SCREEN_HEIGHT - 16) {
      playDinoY = SCREEN_HEIGHT - 16;
    }
  }else if(joyY > 50) {
    playDinoY -= 5; // Move up
    if (playDinoY < 0) {
      playDinoY = 0;
    }
  }

  if (checkCollision(playDinoX, playDinoY, asteroidsX, asteroidsY)) {
    lives--;
    if (lives <= 0 || (millis() - gameStartTime > gameDuration)) {
      isGameActive = false;      
    }
  }

  display.setCursor(0, 0);
  display.print("Lives: ");
  display.print(lives);
  display.setCursor(SCREEN_WIDTH - 40, 0);
  display.print("Time: ");
  display.print((gameDuration - (millis() - gameStartTime)) / 1000);
  display.display();
}

bool checkCollision(int playDinoX, int playDinoY, const int *asteroidsX, const int *asteroidsY) {
  for (int i = 0; i < asteroidCount; i++) {
    if (abs(playDinoX - asteroidsX[i]) < 5 && abs(playDinoY - asteroidsY[i]) < 5) {
      return true;
    }
  }
  return false;
}
