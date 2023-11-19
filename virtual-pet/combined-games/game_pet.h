#ifndef GAME_PET_H
#define GAME_PET_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dino_sprites.h"
#include <Arduino.h>
extern int cursorX, cursorY;
extern int petDinoX, petDinoY;
extern int lastTime;
void startPetGame(Adafruit_SSD1306 &display);
void updatePetGame(Adafruit_SSD1306 &display, int SCREEN_WIDTH, int SCREEN_HEIGHT, int joyX, int joyY, int happiness);
#endif // GAME_PLAY_H
