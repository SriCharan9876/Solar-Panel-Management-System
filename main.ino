#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "solar panel management system"
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD "" 

// Pin definitions
#define DHT_PIN 33
#define LED_PIN 2
#define SERVO_PIN 5
#define LDR_PIN 34
#define BUTTON_PIN 13

// Blynk virtual pins
#define ORIENTATION_BUTTON_PIN V0
#define POWER V1
#define TEMPERATURE V2
#define HUMIDITY V3
#define ILLUMINATION V4

// Initialize components
Servo solarServo;
DHT dht(DHT_PIN, DHT22);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
BlynkTimer timer;

// Constants
const float TEMP_THRESHOLD = 75.0;
const float HUMIDITY_THRESHOLD = 70.0;
const unsigned long SCAN_INTERVAL = 7200000; // 2 hours

// Variables
unsigned long lastScanTime = 0;
boolean onoff = true;
int maxAngle = 0;
int maxIllumination = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing Solar Panel management System...");

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Connect to Blynk
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  
  // Initialize components
  solarServo.attach(SERVO_PIN);
  dht.begin();
  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // OLED initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  // Initial solar panel orientation
  orientPanel();
  lastScanTime = millis();

  // Blynk timer for sending sensor data
  timer.setInterval(2000L, sendSensorData);
}

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int illumination = 4095 - analogRead(LDR_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to Blynk
  Blynk.virtualWrite(TEMPERATURE, temperature);
  Blynk.virtualWrite(HUMIDITY, humidity);
  Blynk.virtualWrite(ILLUMINATION, illumination);
  Blynk.virtualWrite(POWER, (bool)onoff);

  // Update OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.print("Illumination: "); display.println(illumination);
  display.display();

  // Safety checks
  if (humidity > HUMIDITY_THRESHOLD) {
    Serial.println("Rain detected, system paused!");
    display.setCursor(0, 40);
    display.print("Rain detected!");
    display.display();
    onoff = false;
    solarServo.detach();
    digitalWrite(LED_PIN, LOW);
    return;
  }
  if (temperature > TEMP_THRESHOLD) {
    Serial.println("High Temperature, system paused!");
    display.setCursor(0, 40);
    display.print("Temperature exceeded!");
    display.display();
    onoff = false;
    solarServo.detach();
    digitalWrite(LED_PIN, LOW);
    return;
  }

  onoff = true;
  solarServo.attach(SERVO_PIN);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  if (!Blynk.connected()) {
    Blynk.connect();
  }
  Blynk.run();
  timer.run();

  if (millis() - lastScanTime > SCAN_INTERVAL) {
    Serial.println("Auto scan triggered...");
    orientPanel();
    lastScanTime = millis();
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) { // Confirm press
      Serial.println("Manual scan triggered...");
      orientPanel();
    }
  }
}

void orientPanel() {
  maxIllumination = 0;
  maxAngle = 0;

  for (int angle = 0; angle <= 180; angle += 10) {
    solarServo.write(angle);
    delay(500);
    int illumination = 4095 - analogRead(LDR_PIN);
    Serial.print("Orientation: ");
    Serial.print(angle);
    Serial.print(" -- Illumination: ");
    Serial.println(illumination);
    if (illumination > maxIllumination) {
      maxIllumination = illumination;
      maxAngle = angle;
    }
  }
  Serial.print("Optimal angle: "); Serial.println(maxAngle);
  display.setCursor(0, 50);
  display.print("Optimal angle: "); display.print(maxAngle);
  display.display();
  solarServo.write(maxAngle);
}

BLYNK_WRITE(ORIENTATION_BUTTON_PIN) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("Orientation Panel Activated");
    orientPanel();
  }
}
