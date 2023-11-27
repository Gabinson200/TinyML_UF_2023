#include "camera_module.h"

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

const char* ssid = "otthon";
const char* password = "AdavidXfinity+100";
#define PART_BOUNDARY "123456789000000000000987654321"

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// Function to handle the "/food-detected" endpoint
static esp_err_t food_detected_handler(httpd_req_t *req) {
  Serial.println("Food Detected Handler");
  
  // Read the content of the request
  size_t content_length = req->content_len;
  char* content = (char*)malloc(content_length + 1);
  if (content == NULL) {
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }
  httpd_req_recv(req, content, content_length);
  content[content_length] = '\0';

  // Parse the JSON payload
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, content);
  free(content);

  // Check if parsing the JSON was successful
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  // Extract the message field from the JSON
  const char* message = doc["message"];

  // Process the message (you can replace this with your own logic)
  Serial.print("Received Message: ");
  Serial.println(message);

  // Send a response to the client
  const char* response = "Food detection message received!";
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_send(req, response, strlen(response));

  return ESP_OK;
}


static esp_err_t capture_handler(httpd_req_t *req){
    Serial.println("In capture handler");
    Serial.println("Capture image");
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  
    res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
    return res;
}


static esp_err_t page_handler(httpd_req_t *req) {
  Serial.printf("Free heap size before: %u\n", ESP.getFreeHeap());
  esp_err_t res = httpd_resp_send(req, page, sizeof(page));
  if (res != ESP_OK) {
      Serial.println("Failed to send response");
  }
  Serial.printf("Free heap size after: %u\n", ESP.getFreeHeap());
    Serial.println("In page handler");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, page, sizeof(page));
}

static esp_err_t stream_handler(httpd_req_t *req){
    Serial.println("In stream handler");
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        } else {
            
                if(fb->format != PIXFORMAT_JPEG){
                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if(!jpeg_converted){
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                    }
                } else {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
             }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(fb){
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        } else if(_jpg_buf){
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if(res != ESP_OK){
            break;
        }
    }
    return res;
}

void startCameraServer(){
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };
    httpd_uri_t page_uri = {
        .uri       = "/ts",
        .method    = HTTP_GET,
        .handler   = page_handler,
        .user_ctx  = NULL
    };
    httpd_uri_t capture_uri = {
        .uri       = "/capture",
        .method    = HTTP_GET,
        .handler   = capture_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t food_detected_uri = {
        .uri       = "/food-detected",
        .method    = HTTP_POST,
        .handler   = food_detected_handler
    };

    Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &capture_uri);
        httpd_register_uri_handler(camera_httpd, &page_uri);
    }
    // start stream using another webserver
    config.server_port += 1;
    config.ctrl_port += 1;
    Serial.printf("Starting stream server on port: '%d'\n", config.server_port);
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}
