// Solar Monitoring System v2 - Static OTA + Critical SMS Alerts
// Features: Sensors + Inverter Full Display, Auto Scrolling LCD, WiFi + 4G fallback, OTA Updates, Critical SMS Alerts

#include <WiFi.h>
#include <TinyGsmClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>

// ===== Hardware Pin Definitions =====
#define DHTPIN 4
#define DHTTYPE DHT22

#define WIND_SPEED_PIN 25
#define DUST_SENSOR_PIN 32
#define SOLAR_SENSOR_PIN 34
#define WIND_DIRECTION_PIN 35

#define MODEM_PWRKEY 23
#define MODEM_TX 27
#define MODEM_RX 26
#define MODEM_POWER_ON 5

#define I2C_SDA 21
#define I2C_SCL 22

// ===== Setup Objects =====
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);

// ===== WiFi + GSM Credentials =====
const char* ssid = "YourWiFi_SSID";
const char* password = "YourWiFi_PASSWORD";
const char apn[] = "your_apn"; // SIM Card APN

// ===== API Endpoints =====
const char* sensorAPI = "https://solar-monitoring-api.onrender.com/api/sensors/send-sensor-data";
const char* inverterAPI = "https://solar-monitoring-api.onrender.com/api/inverter/send-inverter-data";

// ===== Device Identifiers =====
String plant_id = "Lko-Area-1";
String inverter_id = "Inv-1002";
String customer_id = "672087f520bc5c71160e7306";

// ===== Data Variables =====
unsigned long lastScrollTime = 0;
int currentPage = 0;
const int totalPages = 7;

// ===== Inverter Mock Variables (For Now) =====
String inverterStatus = "Running";
int batterySOC = 85;

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(100);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Solar Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("Booting...");

  dht.begin();
  WiFi.begin(ssid, password);

  lcd.setCursor(0, 2);
  lcd.print("Connecting WiFi...");
  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_attempts < 20) {
    delay(500);
    Serial.print(".");
    wifi_attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    lcd.setCursor(0, 2);
    lcd.print("WiFi Connected   ");
  } else {
    Serial.println("\nWiFi Failed, trying GSM...");
    lcd.setCursor(0, 2);
    lcd.print("WiFi Failed, GSM..");
    connectGSM();
  }

  ArduinoOTA.setHostname("SolarMonitor-Device-001");
  ArduinoOTA.setPassword("solar@123");
  ArduinoOTA.begin();
}

// ===== Main Loop =====
void loop() {
  ArduinoOTA.handle();

  static unsigned long lastDataSent = 0;
  if (millis() - lastDataSent > 30000) { // Send every 30 sec
    lastDataSent = millis();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int dust = analogRead(DUST_SENSOR_PIN);
    int solarRadiation = analogRead(SOLAR_SENSOR_PIN);
    int windDir = analogRead(WIND_DIRECTION_PIN);
    unsigned long windSpeed = pulseIn(WIND_SPEED_PIN, HIGH);

    sendSensorData(temperature, humidity, dust, solarRadiation, windSpeed, windDir);
    sendInverterData(); // Mock inverter data sending
    checkCriticalAlerts();
  }

  if (millis() - lastScrollTime > 5000) { // Change LCD page every 5 seconds
    lastScrollTime = millis();
    lcd.clear();
    displayPage(currentPage);
    currentPage = (currentPage + 1) % totalPages;
  }
}

// ===== GSM Connect =====
void connectGSM() {
  Serial.println("Connecting GSM...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  modem.restart();
  modem.gprsConnect(apn);

  if (modem.isGprsConnected()) {
    Serial.println("GSM Connected!");
  } else {
    Serial.println("GSM Connection Failed");
  }
}

// ===== Send Sensor Data =====
void sendSensorData(float temp, float hum, int dust, int solar, unsigned long windSpeed, int windDir) {
  if (WiFi.status() == WL_CONNECTED || modem.isGprsConnected()) {
    HTTPClient http;
    http.begin(sensorAPI);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"plant_id\":\"" + plant_id + "\","
                     "\"inverter_id\":\"" + inverter_id + "\","
                     "\"customer_id\":\"" + customer_id + "\","
                     "\"humidity\":\"" + String(hum) + "\","
                     "\"temperature\":\"" + String(temp) + "\","
                     "\"dust\":\"" + String(dust) + "\","
                     "\"solar_radiation\":\"" + String(solar) + "\","
                     "\"wind_speed\":\"" + String(windSpeed) + "\","
                     "\"wind_direction\":\"" + String(windDir) + "\"}";

    int httpResponseCode = http.POST(payload);
    Serial.println("Sensor POST: " + String(httpResponseCode));
    http.end();
  }
}

// ===== Send Inverter Data =====
void sendInverterData() {
  if (WiFi.status() == WL_CONNECTED || modem.isGprsConnected()) {
    HTTPClient http;
    http.begin(inverterAPI);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"plant_id\":\"" + plant_id + "\","
                     "\"inverter_id\":\"" + inverter_id + "\","
                     "\"customer_id\":\"" + customer_id + "\","
                     "\"power_output\":\"30\","
                     "\"total_energy_generated\":\"16720.5\","
                     "\"input_voltage\":\"350\","
                     "\"output_voltage\":\"240\","
                     "\"output_current\":\"10\","
                     "\"frequency\":\"50\","
                     "\"efficiency\":\"95.5\","
                     "\"temperature\":\"45\","
                     "\"status\":\"" + inverterStatus + "\","
                     "\"battery_voltage\":\"48\","
                     "\"battery_current\":\"10\","
                     "\"battery_state_of_charge\":\"" + String(batterySOC) + "\","
                     "\"battery_status\":\"Charging\","
                     "\"battery_health\":\"92\","
                     "\"battery_cycles\":\"500\"}";

    int httpResponseCode = http.POST(payload);
    Serial.println("Inverter POST: " + String(httpResponseCode));
    http.end();
  }
}

// ===== Display Pages on LCD =====
void displayPage(int page) {
  switch (page) {
    case 0:
      lcd.setCursor(0, 0); lcd.print("Temp: "); lcd.print(dht.readTemperature()); lcd.print("C");
      lcd.setCursor(0, 1); lcd.print("Humidity: "); lcd.print(dht.readHumidity()); lcd.print("%");
      break;
    case 1:
      lcd.setCursor(0, 0); lcd.print("Solar Rad: "); lcd.print(analogRead(SOLAR_SENSOR_PIN));
      lcd.setCursor(0, 1); lcd.print("Dust: "); lcd.print(analogRead(DUST_SENSOR_PIN));
      break;
    case 2:
      lcd.setCursor(0, 0); lcd.print("PowerOut: 30W");
      lcd.setCursor(0, 1); lcd.print("Eff: 95.5%");
      break;
    case 3:
      lcd.setCursor(0, 0); lcd.print("TotE: 16720.5kWh");
      lcd.setCursor(0, 1); lcd.print("Freq: 50Hz");
      break;
    case 4:
      lcd.setCursor(0, 0); lcd.print("OutputV: 240V");
      lcd.setCursor(0, 1); lcd.print("OutputC: 10A");
      break;
    case 5:
      lcd.setCursor(0, 0); lcd.print("BatV: 48V SOC: "); lcd.print(batterySOC); lcd.print("%");
      lcd.setCursor(0, 1); lcd.print("Bat Status: Charging");
      break;
    case 6:
      lcd.setCursor(0, 0); lcd.print("Inv Temp: 45C");
      lcd.setCursor(0, 1); lcd.print("Bat Health: 92%");
      break;
  }
}

// ===== Critical Alerts Check =====
void checkCriticalAlerts() {
  if (inverterStatus == "Fault") {
    sendCriticalSMS("ALERT: Inverter Fault Detected!");
  }
  if (batterySOC < 20) {
    sendCriticalSMS("ALERT: Battery SOC below 20%");
  }
}

// ===== Send Critical SMS =====
void sendCriticalSMS(String alertText) {
  if (!modem.isGprsConnected()) { // Check GSM connection
    Serial.println("GSM not connected, cannot send SMS");
    return;
  }
  SerialAT.println("AT+CMGF=1"); // Text mode
  delay(100);
  SerialAT.println("AT+CMGS=\"+91xxxxxxxxxx\""); // Your phone number
  delay(100);
  SerialAT.println(alertText);
  delay(100);
  SerialAT.write(26); // Ctrl+Z to send
  delay(5000);
  Serial.println("Critical SMS Sent: " + alertText);
}
