#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "your_network_name"; // your wifi SSID
const char* password = "your_network_password"; // your wifi password

const char* host = "api.thingspeak.com";
const int httpPort = 80;
const char* api_key = "ZXCZXCZXC"; // your Thingspeak Read API
const int channel_number = 12345678; // your Thingspeak channel number

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

String getFieldData(String url, int field_number) {
  HTTPClient http;

  http.begin(url);

  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return "";
      }

      String fieldValue = doc["feeds"][0]["field" + String(field_number)];
      return fieldValue;
    }
  } else {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    return "";
  }

  http.end();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String url = "http://";
    url += host;
    url += "/channels/";
    url += channel_number;
    url += "/feeds.json?api_key=";
    url += api_key;
    url += "&results=1";

    String field1Value = getFieldData(url, 1);
    if (!field1Value.isEmpty()) {
      Serial.println("Value of 'field1': " + field1Value);
    }
    String field2Value = getFieldData(url, 2);
    if (!field1Value.isEmpty()) {
      Serial.println("Value of 'field2': " + field1Value);
    }
  }

  delay(5000); // Fetch data every 5 seconds
}
