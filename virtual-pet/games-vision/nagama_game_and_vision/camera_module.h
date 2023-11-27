#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#define CAMERA_MODEL_WROVER_KIT
#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
#include <ArduinoJson.h>
#include "camera_pins.h"
#include "page.h"

extern const char* ssid;
extern const char* password;

extern httpd_handle_t camera_httpd;
extern httpd_handle_t stream_httpd;

static esp_err_t food_detected_handler(httpd_req_t *req);
static esp_err_t capture_handler(httpd_req_t *req);
static esp_err_t page_handler(httpd_req_t *req);
static esp_err_t stream_handler(httpd_req_t *req);
void startCameraServer();

#endif  // CAMERA_MODULE_H
