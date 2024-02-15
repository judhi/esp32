#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <BluetoothSerial.h>

#define GPS_SERIAL_NUM 1
#define GPS_RX_PIN 34
#define GPS_TX_PIN 12

HardwareSerial GPSSerial(GPS_SERIAL_NUM);
BluetoothSerial SerialBT;

TinyGPSPlus gps; // tinyGPSplus lib by Mikal Hart

String device_name = "JUDHI T-BEAM"; // change this to your own

void setup() {
  SerialBT.begin(device_name);
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  SerialBT.println("Hello GPS");
}

void loop() {
  if (SerialBT.available()) {
    char c = SerialBT.read();
    GPSSerial.write(c);
  }
  
  while (GPSSerial.available()) {
    if (gps.encode(GPSSerial.read())) {
      if (gps.location.isValid()) {
        SerialBT.print("Latitude/Longitude: ");
        SerialBT.print(gps.location.lat(), 6);
        SerialBT.print("/");
        SerialBT.println(gps.location.lng(), 6);
      }
    }
  }
  delay(2000);
}
