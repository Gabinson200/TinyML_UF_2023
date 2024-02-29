#ifndef GAME_FEED_H
#define GAME_FEED_H

#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include "dino_sprites.h"
#include <Arduino.h>

// Function declarations
void startFeedGame(int SCREEN_WIDTH);
void updateFeedGame(Adafruit_SH1106G &display, int joyX, int joyButton, int hunger, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void drawGame(Adafruit_SH1106G &display);

// Definitions
#define DINO_HEIGHT 20
#define DINO_WIDTH 20

// Global variable declarations
extern int foodY;
extern int foodX;
extern const int foodSize;
extern int foodFallSpeed;
extern int feedDinoX;
extern bool pointLeft;

#endif // GAME_FEED_H