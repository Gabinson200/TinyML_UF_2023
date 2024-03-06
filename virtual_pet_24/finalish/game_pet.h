#ifndef GAME_PET_H
#define GAME_PET_H

#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include "dino_sprites.h"
#include <Arduino.h>
#include "Globals.h"
extern int cursorX, cursorY;
extern int petDinoX, petDinoY;
extern int lastTime;
void startPetGame(Adafruit_SH1106G &display);
void updatePetGame(Adafruit_SH1106G &display, int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY);
void drawWhiteBoxes(Adafruit_SH1106G &display);

#endif // GAME_PLAY_H
