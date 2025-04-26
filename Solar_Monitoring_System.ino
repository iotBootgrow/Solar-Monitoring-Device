// Solar Monitoring System Firmware
// WiFi + 4G Fallback | Sensors + Inverter Monitoring | OTA Enabled

// ====== Include Libraries ======
#include <WiFi.h>
#include <TinyGsmClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>

// ====== Hardware Pin Definitions ======
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

// ====== Setup Objects ======
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);

// ====== Credentials ======
const char *ssid = "YourWiFi_SSID";
const char *password = "YourWiFi_PASSWORD";
const char apn[] = "your_apn"; // Replace with SIM card APN

// ====== API Endpoints ======
const char *sensorAPI = "https://solar-monitoring-api.onrender.com/api/sensors/send-sensor-data";
const char *inverterAPI = "https://solar-monitoring-api.onrender.com/api/inverter/send-inverter-data";

// ====== Device Identifiers ======
String plant_id = "Lko-Area-1";
String inverter_id = "Inv-1002";
String customer_id = "672087f520bc5c71160e7306";

// ====== Setup Function ======
void setup()
{
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
    while (WiFi.status() != WL_CONNECTED && wifi_attempts < 20)
    {
        delay(500);
        Serial.print(".");
        wifi_attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi Connected!");
        lcd.setCursor(0, 2);
        lcd.print("WiFi Connected   ");
    }
    else
    {
        Serial.println("\nWiFi Failed, GSM...");
        lcd.setCursor(0, 2);
        lcd.print("WiFi Fail, GSM...");
        connectGSM();
    }

    // Start OTA Updates
    ArduinoOTA.begin();
}

// ====== Loop Function ======
void loop()
{
    ArduinoOTA.handle(); // Check for OTA updates

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int dust = analogRead(DUST_SENSOR_PIN);
    int solarRadiation = analogRead(SOLAR_SENSOR_PIN);
    int windDir = analogRead(WIND_DIRECTION_PIN);
    unsigned long windSpeed = pulseIn(WIND_SPEED_PIN, HIGH);

    // Mock Inverter Data (Can replace with Modbus Reading)
    String inverterPayload = readInverterData();

    sendSensorData(temperature, humidity, dust, solarRadiation, windSpeed, windDir);
    sendInverterData(inverterPayload);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:" + String(temperature) + "C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:" + String(humidity) + "%");
    lcd.setCursor(0, 2);
    lcd.print("Dust:" + String(dust));
    lcd.setCursor(0, 3);
    lcd.print("Solar:" + String(solarRadiation));

    delay(10000); // 10 seconds delay
}

// ====== Connect GSM if WiFi fails ======
void connectGSM()
{
    Serial.println("Connecting GSM...");
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(3000);

    modem.restart();
    modem.gprsConnect(apn);

    if (modem.isGprsConnected())
    {
        Serial.println("GSM Connected!");
    }
    else
    {
        Serial.println("GSM Connection Failed");
    }
}

// ====== Send Sensor Data ======
void sendSensorData(float temp, float hum, int dust, int solar, unsigned long windSpeed, int windDir)
{
    if ((WiFi.status() == WL_CONNECTED) || modem.isGprsConnected())
    {
        HTTPClient http;
        http.begin(sensorAPI);
        http.addHeader("Content-Type", "application/json");

        String payload = "{\"plant_id\":\"" + plant_id + "\","
                                                         "\"inverter_id\":\"" +
                         inverter_id + "\","
                                       "\"customer_id\":\"" +
                         customer_id + "\","
                                       "\"humidity\":\"" +
                         String(hum) + "\","
                                       "\"temperature\":\"" +
                         String(temp) + "\","
                                        "\"dust\":\"" +
                         String(dust) + "\","
                                        "\"solar_radiation\":\"" +
                         String(solar) + "\","
                                         "\"wind_speed\":\"" +
                         String(windSpeed) + "\","
                                             "\"wind_direction\":\"" +
                         String(windDir) + "\"}";

        int httpResponseCode = http.POST(payload);
        Serial.println("Sensor POST Response: " + String(httpResponseCode));
        http.end();
    }
}

// ====== Send Inverter Data ======
void sendInverterData(String inverterPayload)
{
    if ((WiFi.status() == WL_CONNECTED) || modem.isGprsConnected())
    {
        HTTPClient http;
        http.begin(inverterAPI);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(inverterPayload);
        Serial.println("Inverter POST Response: " + String(httpResponseCode));
        http.end();
    }
}

// ====== Mock Inverter Data Reading ======
String readInverterData()
{
    String payload = "{\"plant_id\":\"" + plant_id + "\","
                                                     "\"inverter_id\":\"" +
                     inverter_id + "\","
                                   "\"customer_id\":\"" +
                     customer_id + "\","
                                   "\"power_output\":\"30\","
                                   "\"total_energy_generated\":\"16720.5\","
                                   "\"input_voltage\":\"350\","
                                   "\"output_voltage\":\"240\","
                                   "\"output_current\":\"10\","
                                   "\"frequency\":\"50\","
                                   "\"efficiency\":\"95.5\","
                                   "\"temperature\":\"45\","
                                   "\"status\":\"Running\","
                                   "\"battery_voltage\":\"48\","
                                   "\"battery_current\":\"10\","
                                   "\"battery_capacity\":\"300\","
                                   "\"battery_state_of_charge\":\"85\","
                                   "\"battery_temperature\":\"35\","
                                   "\"battery_status\":\"Charging\","
                                   "\"battery_health\":\"92\","
                                   "\"battery_cycles\":\"500\"}";

    return payload;
}