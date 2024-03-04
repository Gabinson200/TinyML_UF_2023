#include "game_play.h"

const unsigned long happinessIncreaseInterval = 5000; // 5 seconds
int lives = 3;
const int asteroidCount = 5;
int asteroidsX[asteroidCount];
int asteroidsY[asteroidCount];
int asteroidsSpeed[asteroidCount];
int playDinoY = 32;
unsigned long lastHitTime = 0; // Time when the last hit occurred
const unsigned long invincibilityDuration = 1000; // Invincibility duration in milliseconds (e.g., 2000ms = 2 seconds)
int playDinoX = 64;


void startPlayGame(int screenWidth, int screenHeight) {
  Serial.println("in play game");
  isGameActive = true;
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] = random(screenWidth); // Random X position
    asteroidsY[i] = random(screenHeight); // Random Y position
    asteroidsSpeed[i] = random(1, 4); // Random speed
  }
}

void updatePlayGame(Adafruit_SH1106G &display,int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY, bool &isGameActive) {
  display.clearDisplay();
  unsigned long currentTime = millis();

  display.drawBitmap(playDinoX, playDinoY, bitmap_small_dino, 20, 20, SH110X_WHITE);

  //changing and drawing asteroids
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] -= asteroidsSpeed[i]; // Move leftward
    if (asteroidsX[i] < 0) {
      asteroidsX[i] = SCREEN_WIDTH;
      asteroidsY[i] = random(SCREEN_HEIGHT); // New random Y position
    }
    display.fillCircle(asteroidsX[i], asteroidsY[i], 3, SH110X_WHITE); // Draw asteroid
  }

  // Update dinosaur position based on joystick input
  // Update dinosaur position based on joystick input
if (joyX > 100) {
  playDinoX -= 5; // Move left
  if(playDinoX < 0) {
    playDinoX = 0;
  }
} else if (joyX < 28) {
  playDinoX += 5; // Move right
  if (playDinoX > SCREEN_WIDTH - 20) { // Adjusted to ensure dinosaur stays within the screen bounds
    playDinoX = SCREEN_WIDTH - 20;
  }
}

if (joyY > 60) {
  playDinoY -= 5; // Move up
  if (playDinoY < 0) {
    playDinoY = 0;
  }
} else if (joyY < 18) {
  playDinoY += 5; // Move down
  if(playDinoY > SCREEN_HEIGHT - 20) { // Adjusted to ensure dinosaur stays within the screen bounds
    playDinoY = SCREEN_HEIGHT - 20;
  }
}

// Collision and lives logic
if (checkCollision(playDinoX, playDinoY, asteroidsX, asteroidsY)&& lives > 0 && (currentTime - lastHitTime) > invincibilityDuration) {
  lives--;
  lastHitTime = currentTime; 
  if (lives <= 0) {
    isGameActive = false;    
    // Optionally, reset lives here if you want the game to restart with 3 lives every time
    // lives = 3;
  }
}


  display.setCursor(0, 0);
  display.print("Lives: ");
  display.print(lives);
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
