#include "game_play.h"

const int asteroidCount = 5;
int asteroidsX[asteroidCount];
int asteroidsY[asteroidCount];
int asteroidsSpeed[asteroidCount];

void initAsteroids(int screenWidth, int screenHeight) {
    for (int i = 0; i < asteroidCount; i++) {
        asteroidsX[i] = random(screenWidth); // Random X position
        asteroidsY[i] = random(screenHeight); // Random Y position
        asteroidsSpeed[i] = random(1, 4); // Random speed
    }
}


void updateAsteroids(Adafruit_SSD1306 &display,int SCREEN_WIDTH, int SCREEN_HEIGHT) {
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] -= asteroidsSpeed[i]; // Move leftward
    if (asteroidsX[i] < 0) {
      asteroidsX[i] = SCREEN_WIDTH;
      asteroidsY[i] = random(SCREEN_HEIGHT); // New random Y position
    }
    display.fillCircle(asteroidsX[i], asteroidsY[i], 3, SSD1306_WHITE); // Draw asteroid
  }
}

bool checkCollision(int dinosaurX, int dinosaurY, const int *asteroidsX, const int *asteroidsY) {
  for (int i = 0; i < asteroidCount; i++) {
    if (abs(dinosaurX - asteroidsX[i]) < 5 && abs(dinosaurY - asteroidsY[i]) < 5) {
      return true;
    }
  }
  return false;
}
