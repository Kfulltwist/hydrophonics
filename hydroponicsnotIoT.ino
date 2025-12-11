#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor and relay pin assignments
#define PH_SENSOR A0
#define SOIL_MOISTURE_SENSOR A1
#define WATER_LEVEL_SENSOR 4
#define RELAY_PIN 7

// Safe pH range for pumping
#define PH_MIN 5.5
#define PH_MAX 6.5

// Soil moisture threshold (above this, pump turns off)
#define SOIL_MOISTURE_WET 70  // Example: 70% is very wet

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    pinMode(WATER_LEVEL_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure pump is off initially
}

void loop() {
    // Read pH sensor
    float rawValue = analogRead(PH_SENSOR);
    float phValue = (rawValue - 140) * (14.0 / 600); // adjust based on real time values
    
    // Read soil moisture sensor
    int soilMoisture = analogRead(SOIL_MOISTURE_SENSOR);
    int moisturePercent = map(soilMoisture, 1023, 0, 0, 100);
    
    // Read water level sensor
    int waterLevel = digitalRead(WATER_LEVEL_SENSOR);
    
    // Determine pump status
    String pumpStatus = "OFF";
    if (phValue >= PH_MIN && phValue <= PH_MAX && moisturePercent < SOIL_MOISTURE_WET) {
        if (waterLevel == LOW) {
            digitalWrite(RELAY_PIN, HIGH);
            pumpStatus = "ON ";
        } else {
            digitalWrite(RELAY_PIN, LOW);
        }
    } else {
        digitalWrite(RELAY_PIN, LOW);
    }
    
    // Determine water status
    String waterStatus = (phValue < PH_MIN || phValue > PH_MAX) ? "BAD!" : "OK";
    
    // Display formatted data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pH:"); lcd.print(phValue, 2);
    lcd.setCursor(10, 0);
    lcd.print("P:"); lcd.print(pumpStatus);
    
    lcd.setCursor(0, 1);
    lcd.print("M:"); lcd.print(moisturePercent); lcd.print("%");
    lcd.setCursor(10, 1);
    lcd.print("W:"); lcd.print(waterStatus);
    
    // Print to serial monitor for debugging
    Serial.print("pH: "); Serial.print(phValue, 2);
    Serial.print(" | Moisture: "); Serial.print(moisturePercent); Serial.print("%");
    Serial.print(" | Water Level: "); Serial.print(waterLevel);
    Serial.print(" | Pump: "); Serial.println(pumpStatus);
    
    delay(2000); // Wait for 2 seconds before next reading
}
