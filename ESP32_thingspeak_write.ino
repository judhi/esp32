// Uploading data to Thingspeak using Liligo T-Beam
// by Judhi Prasetyo
// inspired by https://randomnerdtutorials.com/esp32-thingspeak-publish-arduino/

#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid =  "....";   // WiFi network SSID 
const char* password = "...u";   // WiFi network password

WiFiClient  client;

unsigned long channel = ....; // your Thingspeak channel number
const char * API = "....."; // your channel write API

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold temperature readings
float temp;

void setup() {
  Serial.begin(115200);  //Initialize serial 
  Serial.println("T-Beam ready");
  WiFi.mode(WIFI_STA);   
  Serial.println("WiFi ready");
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("Thingspeak client ready");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    // Get a new temperature reading
    temp = random(100); // dummy sensor, throwing random number 0-100
    Serial.print("Temperature (ºC): ");
    Serial.println(temp);
       
    // Write to our ThingSpeak channel on field 1 (up to 8 fields in a channel)
    // and get the response code
    int field = 1;
    int resp = ThingSpeak.writeField(channel, field, temp, API);

    if(resp == 200){ // HTTP Ok
      Serial.println("Channel update successful.");
    }
    else{         // Others
      Serial.println("Problem updating channel. HTTP error code " + String(resp));
    }
    lastTime = millis();
  }
}
