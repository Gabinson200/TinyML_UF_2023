#ifndef GAME_FEED_H
#define GAME_FEED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dino_sprites.h"
#include <Arduino.h>

// Function declarations
void startFeedGame(int SCREEN_WIDTH);
void updateFeedGame(Adafruit_SSD1306 &display, int joyX, int joyButton, int hunger, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void drawGame(Adafruit_SSD1306 &display);

// Definitions
#define DINO_HEIGHT 20
#define DINO_WIDTH 20

// Global variable declarations
extern bool isGameActive;
extern unsigned long gameStartTime;
extern const unsigned long gameDuration;
extern int foodY;
extern int foodX;
extern const int foodSize;
extern int foodFallSpeed;
extern int feedDinoX;
extern bool pointLeft;

#endif // GAME_FEED_H
