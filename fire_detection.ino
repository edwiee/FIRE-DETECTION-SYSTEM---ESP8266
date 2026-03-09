/*
 * ============================================================
 *  FIRE DETECTION SYSTEM - ESP8266
 *  Author  : edwiee
 *  GitHub  : https://github.com/[YourUsername]/fire-detection-esp8266
 *  License : MIT
 * ============================================================
 *
 *  Components:
 *    - ESP8266 NodeMCU (Lolin/WeMos D1 Mini)
 *    - DHT11 Temperature & Humidity Sensor  (GPIO D4 / Pin 2)
 *    - MQ-2 Smoke / Gas Sensor              (Analog A0)
 *    - Active Buzzer                         (GPIO D6 / Pin 12)
 *    - Red LED (Flame Alert)                 (GPIO D7 / Pin 13)
 *    - Green LED (System OK)                 (GPIO D5 / Pin 14)
 *    - 16x2 I2C LCD Display                  (SDA=D2, SCL=D1)
 *    - Push Button (Reset / Silence)         (GPIO D3 / Pin 0)
 *
 *  IDE      : Arduino IDE 2.x
 *  Board Pkg : esp8266 by ESP8266 Community v3.1.2
 *  Libraries : DHT sensor library (Adafruit), 
 *              LiquidCrystal_I2C,
 *              ESP8266WiFi (built-in)
 * ============================================================
 */

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// ---------- Pin Definitions ----------
#define DHT_PIN        2    // D4 on NodeMCU
#define DHT_TYPE       DHT11
#define SMOKE_PIN      A0   // Analog input
#define BUZZER_PIN     12   // D6
#define LED_RED_PIN    13   // D7
#define LED_GREEN_PIN  14   // D5
#define BUTTON_PIN     0    // D3 (built-in FLASH btn)

// ---------- Threshold Settings ----------
#define TEMP_THRESHOLD      50.0   // °C  — fire warning level
#define SMOKE_THRESHOLD     400    // ADC value (0-1023)
#define BUZZER_FREQ         1000   // Hz (tone frequency)
#define SAMPLE_INTERVAL     1000   // ms between readings

// ---------- WiFi Credentials (optional alerting) ----------
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

// ---------- Objects ----------
DHT             dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Adjust address if needed

// ---------- State Variables ----------
bool  alertActive     = false;
bool  alertSilenced   = false;
unsigned long lastSampleTime = 0;

// ============================================================
void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(BUZZER_PIN,    OUTPUT);
  pinMode(LED_RED_PIN,   OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN,    INPUT_PULLUP);

  // Safe initial state
  digitalWrite(BUZZER_PIN,    LOW);
  digitalWrite(LED_RED_PIN,   LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);

  // Init sensors
  dht.begin();

  // Init LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fire Detection");
  lcd.setCursor(0, 1);
  lcd.print("System  v1.0");
  delay(2000);
  lcd.clear();

  // Optional WiFi
  connectWiFi();

  Serial.println(F("=== Fire Detection System Ready ==="));
}

// ============================================================
void loop() {
  unsigned long now = millis();

  // Read sensors at defined interval
  if (now - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = now;
    readAndEvaluate();
  }

  // Handle silence/reset button (active LOW)
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);  // debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      handleButtonPress();
    }
  }

  // Pulse buzzer if alert is active and not silenced
  if (alertActive && !alertSilenced) {
    tone(BUZZER_PIN, BUZZER_FREQ, 200);
  }
}

// ============================================================
void readAndEvaluate() {
  // ---- Read DHT11 ----
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();  // Celsius

  // ---- Read MQ-2 ----
  int smokeLevel = analogRead(SMOKE_PIN);

  // ---- Validate DHT reading ----
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("[WARN] DHT read failed, skipping."));
    lcd.setCursor(0, 1);
    lcd.print("Sensor Error!   ");
    return;
  }

  // ---- Serial log ----
  Serial.printf("[DATA] Temp: %.1f°C | Humidity: %.1f%% | Smoke ADC: %d\n",
                temperature, humidity, smokeLevel);

  // ---- Evaluate thresholds ----
  bool tempAlert  = (temperature >= TEMP_THRESHOLD);
  bool smokeAlert = (smokeLevel  >= SMOKE_THRESHOLD);

  if (tempAlert || smokeAlert) {
    triggerAlert(temperature, smokeLevel, tempAlert, smokeAlert);
  } else {
    clearAlert(temperature, smokeLevel);
  }

  // ---- Update LCD ----
  updateLCD(temperature, humidity, smokeLevel, tempAlert || smokeAlert);
}

// ============================================================
void triggerAlert(float temp, int smoke, bool isTemp, bool isSmoke) {
  alertActive   = true;
  alertSilenced = false;

  digitalWrite(LED_RED_PIN,   HIGH);
  digitalWrite(LED_GREEN_PIN, LOW);

  Serial.println(F("[ALERT] *** FIRE / SMOKE DETECTED ***"));
  if (isTemp)  Serial.printf("  High Temperature: %.1f°C\n", temp);
  if (isSmoke) Serial.printf("  High Smoke Level : %d\n", smoke);
}

// ============================================================
void clearAlert(float temp, int smoke) {
  if (alertActive) {
    Serial.println(F("[INFO] Conditions normal. Alert cleared."));
  }
  alertActive   = false;
  alertSilenced = false;

  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN,    LOW);
  digitalWrite(LED_RED_PIN,   LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
}

// ============================================================
void handleButtonPress() {
  if (alertActive) {
    alertSilenced = true;
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println(F("[INFO] Buzzer silenced by user."));
    lcd.setCursor(0, 1);
    lcd.print("Buzzer Silenced ");
    delay(1500);
  }
  while (digitalRead(BUTTON_PIN) == LOW);  // wait for release
}

// ============================================================
void updateLCD(float temp, float humidity, int smoke, bool alarm) {
  lcd.setCursor(0, 0);
  if (alarm) {
    lcd.print("!! FIRE ALERT !!");
  } else {
    lcd.printf("T:%.1fC  H:%.0f%%  ", temp, humidity);
  }

  lcd.setCursor(0, 1);
  lcd.printf("Smoke:%-4d %s", smoke, alarm ? "[!]" : "[OK]");
}

// ============================================================
void connectWiFi() {
  if (String(WIFI_SSID) == "YOUR_SSID") return;  // Skip if not configured

  Serial.printf("\n[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println(F("\n[WiFi] Connection failed — offline mode."));
  }
}
