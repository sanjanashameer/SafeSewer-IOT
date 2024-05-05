#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dht.h"

#define sensor A1
#define Highpulse 90

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Initialize OLED display
dht DHT;

unsigned long lastTemperatureTime = 0;
unsigned long lastPulseTime = 0;
bool displayTemperature = true;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize OLED display with I2C address 0x3C
  display.clearDisplay();
}

void loop() {
  unsigned long currentTime = millis();

  // Display temperature for 5 seconds
  if (displayTemperature && (currentTime - lastTemperatureTime >= 5000)) {
    display.clearDisplay();
    DHT.read11(A0); // Read temperature and humidity
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Humidity: ");
    display.print(DHT.humidity);
    display.println("%");
    display.setCursor(0, 20);
    display.print("Temperature: ");
    display.print(DHT.temperature);
    display.println("C");
    display.display();
    lastTemperatureTime = currentTime;
    displayTemperature = false;
    
    // Print temperature and humidity to serial monitor
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.println("C");
    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);
    Serial.println("%");
  }

  // Display pulse for 10 seconds
  if (!displayTemperature && (currentTime - lastPulseTime >= 7000)) {
    display.clearDisplay();
    int Svalue = analogRead(sensor);
    int value = map(Svalue, 0, 1024, 0, 45);
    int y = 60 - value;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Pulse: ");
    display.print(value);
    display.display();
    lastPulseTime = currentTime;
    displayTemperature = true;

    // Print pulse value to serial monitor
    Serial.print("Pulse: ");
    Serial.println(value);
  }
}