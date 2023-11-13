
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dino_sprites.h"

#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT        64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define JOY_X 35 // Joystick X-axis analog pin
#define JOY_Y 32 // Joystick Y-axis analog pin
#define JOY_BTN 13 // Joystick button pin

const int buttonWidth = 32;
const int buttonHeight = 12;
const int buttonY = 0;
const int buttonX[4] = {0, 32, 64, 96};
const char* menuLabels[4] = {"Feed", "Pet", "Nap", "Play"};
bool selected[4] = {false, false, false, false};
int joyX = 0;
int joyY = 0;
int joyButton = LOW;
int menuCount = -1;


int dinosaurX = 0;
int dinosaurY = 0;
int dinosaurSpeed = 2;

int hunger = 100;
int happiness = 48;
int tired = 100;

unsigned char* dino_right[] = {bitmap_right_tail_up, bitmap_right_tail_down};
unsigned char* dino_left[] = {bitmap_left_tail_up, bitmap_left_tail_down};

unsigned char* current_dino_sprite;

const int asteroidCount = 5;
int asteroidsX[asteroidCount];
int asteroidsY[asteroidCount];
int asteroidsSpeed[asteroidCount];
int lives = 3; // Number of lives
unsigned long gameStartTime;
const unsigned long gameDuration = 60000; // Game duration in milliseconds (60 seconds)


bool inGame = false;
unsigned long startTime;
const unsigned long happinessIncreaseInterval = 5000; // 5 seconds

// Initialize asteroids
void initAsteroids() {
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsX[i] = random(SCREEN_WIDTH); // Random X position across the screen width
    asteroidsY[i] = -10; // Start off-screen at the top
    asteroidsSpeed[i] = random(1, 4); // Random speed
  }
}
// Move asteroids
void updateAsteroids() {
  for (int i = 0; i < asteroidCount; i++) {
    asteroidsY[i] += asteroidsSpeed[i]; // Move downwards
    if (asteroidsY[i] > SCREEN_HEIGHT + 10) { // Reset asteroid when it goes off-screen
      asteroidsY[i] = -10;
      asteroidsX[i] = random(SCREEN_WIDTH);
    }
    display.fillCircle(asteroidsX[i], asteroidsY[i], 3, SSD1306_WHITE); // Draw asteroid
  }
}

// Check for collision
bool checkCollision() {
  for (int i = 0; i < asteroidCount; i++) {
    if (abs(dinosaurX - asteroidsX[i]) < 5 && abs(dinosaurY - asteroidsY[i]) < 5) {
      return true;
    }
  }
  return false;
}


void setup() {
  pinMode(JOY_BTN, INPUT_PULLUP);

  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

  // Display menu buttons
  for (int i = 0; i < 4; i++) {
    display.drawRect(buttonX[i], buttonY, buttonWidth, buttonHeight, SSD1306_WHITE);
  }
  //Draw Menu Text
  displayMenuText();
  display.display();

}

void loop() {
  if (menuCount == 3 && !inGame) { // Enter the game when 'Play' is selected
    inGame = true;
    initAsteroids();
    dinosaurX = SCREEN_WIDTH / 2;
    dinosaurY = SCREEN_HEIGHT / 2;
    startTime = millis();
    gameStartTime = millis();
    lives = 3; // Reset lives
  }


  if (inGame) {
    // Game logic

    joyX = analogRead(JOY_X);
    joyY = analogRead(JOY_Y);

    // Update dinosaur position based on joystick input
    dinosaurX += map(joyX, 0, 1023, -2, 2);
    dinosaurY += map(joyY, 0, 1023, -2, 2);
    

    // Boundaries for dinosaur
    dinosaurX = constrain(dinosaurX, 0, SCREEN_WIDTH - 16);
    dinosaurY = constrain(dinosaurY, 0, SCREEN_HEIGHT - 16);

    // Update game state
    updateAsteroids();
    if (checkCollision()) {
      lives--;
      if (lives <= 0 || (millis() - gameStartTime > gameDuration)) {
        inGame = false;
        menuCount = -1; // Reset to main menu
        // Reset the dinosaur sprite for the main menu
        current_dino_sprite = (happiness > 50) ? dino_right[0] : dino_left[0];
        dinosaurX = 0; // Reset the initial X position of the dinosaur
        dinosaurY = 0; // Reset the initial Y position of the dinosaur
}
    }

    // Increase happiness over time
    if (millis() - gameStartTime > gameDuration) {
      inGame = false;
    }

    // Draw game elements
    display.clearDisplay();
    updateAsteroids();
    // display.drawBitmap(dinosaurX, dinosaurY, current_dino_sprite, 16, 16, SSD1306_WHITE);
    // display.setCursor(0, 0);
    // display.print("Lives: ");
    // display.print(lives);
    // display.setCursor(SCREEN_WIDTH - 40, 0);
    // display.print("Time: ");
    // display.print((gameDuration - (millis() - gameStartTime)) / 1000);

    // display.display();  
    display.drawBitmap(dinosaurX, dinosaurY, dino16x16, 16, 16, SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("Lives: ");
    display.print(lives);
    display.setCursor(SCREEN_WIDTH - 40, 0);
    display.print("Time: ");
    display.print((gameDuration - (millis() - gameStartTime)) / 1000);

    display.display();
    } 
    else {
  display.drawBitmap(dinosaurX, dinosaurY, current_dino_sprite, 128, 64, SSD1306_WHITE);
  joyX = map(analogRead(JOY_X), 0, 1023, 0, 32);
  joyY = map(analogRead(JOY_Y), 0, 1023, 0, 16);
  joyButton = digitalRead(JOY_BTN);

  //Serial.print(joyX);
  //Serial.print(", ");
  //Serial.println(joyY);

  if(!joyButton){
    happiness+=1;
    Serial.println(happiness);
  }

  if(joyX > 100){
    menuCount++;
    if(menuCount > 3){
      menuCount = 0;
    }
  }else if(joyX < 28){
    menuCount--;
    if(menuCount < 0){
      menuCount = 3;
    }
  }

  display.clearDisplay();

  for (int i = 0; i < 4; i++) {
    if (i == menuCount) {
      // Flash the selected button by toggling between black and white
      static bool flashState = false;
      display.drawRect(buttonX[i], buttonY, buttonWidth, buttonHeight, flashState ? SSD1306_BLACK : SSD1306_WHITE);
      flashState = !flashState;
    } else {
      display.drawRect(buttonX[i], buttonY, buttonWidth, buttonHeight, SSD1306_WHITE);
    }
 }


  //get dino sprite to draw
  if(dinosaurSpeed >= 0){
    current_dino_sprite = dino_right[happiness > 50 ? 0 : 1];
  }else{
    current_dino_sprite = dino_left[happiness > 50 ? 0 : 1];
  }

  //flipBitmapVertical(current_dino_sprite, 128, 64);
  display.drawBitmap(dinosaurX, dinosaurY, current_dino_sprite, 128, 64, SSD1306_WHITE);
  

  // Move the dinosaur horizontally
  dinosaurX += dinosaurSpeed;

  // Check for boundaries and reverse direction
  if (dinosaurX <= -64 || dinosaurX >= SCREEN_WIDTH - 64) {
    dinosaurSpeed = -dinosaurSpeed;
  }

  displayMenuText();
  display.display();
  delay(10); // this speeds up the simulation
  }

}

void displayMenuText(){
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(4, 3);
  display.println("Feed");

  display.setCursor(40, 3);
  display.println("Pet");

  display.setCursor(72, 3);
  display.println("Nap");

  display.setCursor(101, 3);
  display.println("Play");
}

/*
void flashBitmap(static unsigned char bit_map){
  for (int i = 0; i < 8; i++) {
    bit_map[i] = ~bit_map[i];
  }
}

void flipBitmap(static unsigned char* bit_map, int size){
  for (int i = 0; i < size / 2; i++) {
    unsigned char temp = bit_map[i];
    bit_map[i] = bit_map[size - 1 - i];
    bit_map[size - 1 - i] = temp;
  }
}
*/

/*
void flipBitmapVertical(unsigned char* bitmap, int width, int height) {
  unsigned char tempBuffer[1024];
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int srcIndex = row * width + col;
      int destIndex = (height - 1 - row) * width + col;
      tempBuffer[destIndex] = bitmap[srcIndex];
    }
  }

  // Copy the flipped data back to the original bitmap in PROGMEM
  for (int i = 0; i < width * height; i++) {
    bitmap[i] = tempBuffer[i];
  }
}
*/

