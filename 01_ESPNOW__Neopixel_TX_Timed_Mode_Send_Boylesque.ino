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
int Greenchange = 0;
int Redchange = 0;
int Bluechange = 0;

// REPLACE WITH RECEIVER MAC Address
uint8_t ESP_1[] = {0xEC, 0xFA, 0xBC, 0x32, 0x14, 0x32};//34:94:54:34:F5:F0
uint8_t ESP_2[] = {0x5C, 0xCF, 0x7F, 0xB2, 0xAD, 0xD9};//34:94:54:35:29:58
uint8_t ESP_3[] = {0xF4, 0xCF, 0xA2, 0xF6, 0x78, 0x66};// 34:94:54:35:29:30
uint8_t ESP_4[] = {0xF4, 0xCF, 0xA2, 0xF6, 0x72, 0xA3};//34:94:54:35:39:8C
uint8_t ESP_5[] = {0xF4, 0xCF, 0xA2, 0xF6, 0x72, 0xB6};//34:94:54:36:0F:F8

//Timers
unsigned long time_now = 0;
unsigned long time_change = 10000;  // seconds * 1000(millis)
unsigned long wait_state = 10000;

// Mode buttons
//const int switch_LED_off = 15;  //Switch off Leds
//const int switch_LED_white = 23; //Switch Led's to all white
const int switch_LEDs = 0; // Switch Led's to pride colours
const int power_On_LED = LED_BUILTIN;     // 

// Button start up states
 int LEDs_On = 0;  //Switch off Leds
 int active_mode = 0; //store for current mode
 
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
 int green = Greenchange;
 int red = Redchange;
 int blue = Bluechange;
 int send_mode = 0;
} struct_message;

// Create a struct_message called myData
struct_message myData;



unsigned long lastTime = 0;  
unsigned long timerDelay = 500;  // send readings timer
unsigned long longtimerDelay = 3500;  // send readings timer
unsigned long colour_Change = 30000;

// callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
 

void colour_White(){
  Greenchange = 255;
    Redchange = 255;
    Bluechange =255;

    myData.green = 255;
    myData.red = 255;
    myData.blue = 255;

    
}



 
void setup() {
  //define input -outputs
      //Init Mode button
         pinMode(switch_LEDs, INPUT);
     //define LED outputs   
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  
   // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
    esp_now_add_peer(ESP_1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(ESP_2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(ESP_3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(ESP_4, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

 
  // set leds off
     myData.green = 0;
    myData.red = 0;
    myData.blue = 0;

  // Send message via ESP-NOW
      esp_now_send(0, (uint8_t *) &myData, sizeof(struct_message));

  //Show onboard led for a short period to show power up
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);

}
 
void loop() {

   LEDs_On = digitalRead(switch_LEDs);

  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
 
  if (LEDs_On == LOW ) { active_mode = 1; }; 
 
 

 
    
  if (active_mode == 1) {
    Serial.print("currentMode: ");
    Serial.println("White");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    colour_White();
    esp_now_send(0, (uint8_t *) &myData, sizeof(struct_message));
    delay(25000);
    myData.send_mode = 3;
    esp_now_send(0, (uint8_t *) &myData, sizeof(struct_message));
    
}
     delay(100);

}
