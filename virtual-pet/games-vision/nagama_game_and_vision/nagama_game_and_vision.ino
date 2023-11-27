#define CAMERA_MODEL_WROVER_KIT
#include "camera_module.h"
#include <SPI.h>
#include <Wire.h>
#include "dino_sprites.h"
#define PART_BOUNDARY "123456789000000000000987654321"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "game_feed.h"
#include "game_play.h"
#include "game_pet.h"


static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

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


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
#if defined(CAMERA_MODEL_M5STACK_WIDE)  || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Code for games
  //--------------------------------------------------------------
  pinMode(JOY_BTN, INPUT_PULLUP);

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
  joyX = map(analogRead(JOY_X), 0, 1023, 0, 32);
  joyY = map(analogRead(JOY_Y), 0, 1023, 0, 16);
  joyButton = digitalRead(JOY_BTN);


  if(selected[0]) {
    updateFeedGame(display, joyX, joyButton, hunger, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (joyButton == LOW) {
      selected[0] = false;
    }
  
  }else if(selected[1]){
    updatePetGame(display, SCREEN_WIDTH, SCREEN_HEIGHT, joyX, joyY, happiness);
    if (joyButton == LOW) {
      selected[1] = false;
    }

  }else if(selected[3]){
    updatePlayGame(display,SCREEN_WIDTH, SCREEN_HEIGHT, joyX, joyY);
    if (joyButton == LOW) {
      selected[3] = false;
    }
  
  }else if(selected[2]){
    startCameraServer();
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
    if (joyButton == LOW) {
      selected[2] = false;
    }

  }else {

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
  

    // Example of triggering the game
    if (menuCount == 0 && joyButton == LOW ) { // Assuming "Feed" is the first menu item
      startFeedGame(SCREEN_WIDTH);
      selected[0] = true;
    }

    if (menuCount == 3 && joyButton == LOW ) { // Assuming "Feed" is the first menu item
      startPlayGame(SCREEN_WIDTH, SCREEN_HEIGHT);
      selected[3] = true;
    }

    if (menuCount == 1 && joyButton == LOW ) { // Assuming "Feed" is the first menu item
      //startPetGame(display);
      selected[1] = true;
    }



  }
  
  display.display();
  delay(10); // this speeds up the simulation
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

