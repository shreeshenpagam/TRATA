#include <TinyGPS++.h>

TinyGPSPlus gps;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);     // Monitor
  Serial1.begin(9600);    // GPS
  Serial2.begin(9600);    // GSM

  Serial.println("System Starting...");
  delay(3000);

  // Test GSM
  Serial2.println("AT");
  delay(1000);
  Serial2.println("AT+CMGF=1"); // SMS text mode
  delay(1000);
}

void loop() {

  // Read GPS data
  while (Serial1.available()) {
    gps.encode(Serial1.read());
  }

  // If GPS location is valid
  if (gps.location.isValid()) {

    float lat = gps.location.lat();
    float lon = gps.location.lng();

    Serial.print("Lat: ");
    Serial.println(lat, 6);
    Serial.print("Lon: ");
    Serial.println(lon, 6);

    // Send SMS every 20 seconds
    if (millis() - lastSend > 20000) {
      sendSMS(lat, lon);
      lastSend = millis();
    }
  }
}

void sendSMS(float lat, float lon) {
  Serial.println("Sending SMS...");

  Serial2.println("AT+CMGF=1");
  delay(1000);

  Serial2.println("AT+CMGS=\"+918308032572\""); 
  delay(1000);

  Serial2.print("EMERGENCY ALERT Women Safety system Triggered:\n");
  Serial2.print("https://maps.google.com/?q=");
  Serial2.print(lat, 6);
  Serial2.print(",");
  Serial2.print(lon, 6);

  delay(500);

  Serial2.write(26); 
  delay(5000);

  Serial.println("SMS Sent");
}