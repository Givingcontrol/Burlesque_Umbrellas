/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  ****************************************************************************************
*esp-now
*esp01 master/transmitter sending mode and colour information to a host of receivers 
*       receivers mounted in on neopixel rings attached to umbrellas
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

#include<Adafruit_NeoPixel.h>
#define LED_PIN 0 // WS2812 data pin
#define LED_COUNT 24 // Number of LEDs
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
int sat = 255;
int r =0;
int g =0;
int b =0;
int m = 0;

// timers 
int pixel_change = 100;
int colour_change = 2500;
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int green ;
 int red ;
 int blue;
 int sent_mode = 0;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac,  uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if ( myData.sent_mode == 3){colour_fade();}
  if ( myData.sent_mode == 1){colour_off();}
  else{
        g = myData.green;
        r = myData.red ;
        b = myData.blue ;
        m = myData.sent_mode ;
          
        strip.setBrightness(255);
        uint32_t color = strip.Color(r, g, b);
        strip.fill(color, 0, LED_COUNT);
        strip.show();
      } 
  
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("sent_mode: ");
  Serial.println(m);
  Serial.print("Green: ");
  Serial.println(g);
  Serial.print("Red: ");
  Serial.println(r);
   Serial.print("Blue: ");
  Serial.println(b);
   Serial.println();

 
}

// Fill along the length of the strip in various colors...
void colour_fade(){
  Serial.print("Colour Fade ");
  colorWipe(strip.Color( 0,   255,   0), pixel_change); // Red
  delay(colour_change);
  colorWipe(strip.Color(124, 245  ,   0), pixel_change); // Orange
  delay(colour_change);
  colorWipe(strip.Color(247,   245,   0), pixel_change); // Yellow
  delay(colour_change);
  colorWipe(strip.Color(  255, 0,   0), pixel_change); // Green
  delay(colour_change);
  colorWipe(strip.Color(  3,   3, 255), pixel_change); // Blue
  delay(colour_change);
  colorWipe(strip.Color(0,   255,   250), pixel_change); // purple
   delay(colour_change);
    if (m == 1){colour_off();}
  }

void colour_off(){
  strip.setBrightness(0);
        uint32_t color = strip.Color(0, 0, 0);
        strip.fill(color, 0, LED_COUNT);
        strip.show();
    
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
  void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

//define LED outputs   
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {

  //esp_now_register_recv_cb(OnDataRecv);
}
  
