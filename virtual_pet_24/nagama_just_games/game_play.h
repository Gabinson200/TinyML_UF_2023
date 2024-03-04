#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include <Adafruit_SSD1306.h>
#include "dino_sprites.h"
#include <Arduino.h>

// Function declarations
//void updatePlayGame(Adafruit_SSD1306 &display, int joyX, int joyButton, int hunger, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void drawGame(Adafruit_SH1106G &display);
void startPlayGame(int screenWidth, int screenHeight);
void updatePlayGame(Adafruit_SH1106G &display,int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY, bool &isGameActive);
bool checkCollision(int dinosaurX, int dinosaurY, const int *asteroidsX, const int *asteroidsY);
// Definitions
#define DINO_HEIGHT 20
#define DINO_WIDTH 20

// Global variable declarations

extern bool isGameActive;
extern int playDinoY;
extern int playDinoX;
extern bool pointLeft;
extern int lives;
extern const unsigned long happinessIncreaseInterval;
extern const int asteroidCount;
extern int asteroidsX[];
extern int asteroidsY[];
extern int asteroidsSpeed[];

#endif // GAME_PLAY_H
