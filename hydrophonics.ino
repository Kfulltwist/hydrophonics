#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <LiquidCrystal_I2C.h>

// Replace with your Blynk authentication token, WiFi credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// SoftwareSerial for ESP8266-01 (TX=2, RX=3)
SoftwareSerial EspSerial(2, 3); // RX, TX
#define ESP8266_BAUD 9600

BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor and relay pin assignments
#define PH_SENSOR A0
#define SOIL_MOISTURE_SENSOR A1
#define WATER_LEVEL_SENSOR 4
#define RELAY_PIN 7

void setup() {
    Serial.begin(9600);
    EspSerial.begin(ESP8266_BAUD);
    Blynk.begin(auth, EspSerial, ssid, pass);
    lcd.begin();
    lcd.backlight();
    pinMode(WATER_LEVEL_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    timer.setInterval(2000L, sendSensorData);
}

void sendSensorData() {
    float phValue = analogRead(PH_SENSOR) * (14.0 / 1023.0);
    int soilMoisture = analogRead(SOIL_MOISTURE_SENSOR);
    int waterLevel = digitalRead(WATER_LEVEL_SENSOR);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pH: "); lcd.print(phValue);
    lcd.setCursor(0, 1);
    lcd.print("Moist: "); lcd.print(soilMoisture);
    
    Blynk.virtualWrite(V1, phValue);
    Blynk.virtualWrite(V2, soilMoisture);
    Blynk.virtualWrite(V3, waterLevel);
    
    if (waterLevel == LOW) {
        digitalWrite(RELAY_PIN, HIGH);
    } else {
        digitalWrite(RELAY_PIN, LOW);
    }
}

void loop() {
    Blynk.run();
    timer.run();
}
