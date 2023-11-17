#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Function declarations
void initAsteroids(int screenWidth, int screenHeight);
void updateAsteroids(Adafruit_SSD1306 &display,int screenWidth, int screenHeight);
bool checkCollision(int dinosaurX, int dinosaurY, const int *asteroidsX, const int *asteroidsY);

// Global variable declarations
extern const int asteroidCount;
extern int asteroidsX[];
extern int asteroidsY[];
extern int asteroidsSpeed[];

#endif // GAME_PLAY_H
