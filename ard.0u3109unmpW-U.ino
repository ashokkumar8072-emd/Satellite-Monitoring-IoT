#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define BUZZER_PIN 18
#define LED_PIN 19

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "ashok";
const char* password = "Ashok@07";
String apiKey = "FO3MMUWKOWUF9SIQ";

DHT dht(DHTPIN, DHTTYPE);
MPU6050 mpu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long lastUpload = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(21, 22);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED NOT FOUND");
    while (1);
  }

  mpu.initialize();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Satellite Monitor");
  display.display();

  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  float pitch = atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0 / PI;
  float roll  = atan2(ay_g, sqrt(ax_g * ax_g + az_g * az_g)) * 180.0 / PI;

  String status = "NORMAL";

  // ================= TEMPERATURE ALERT =================
  if (temperature > 35)
  {
    status = "TEMP HIGH WARNING";

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);   // Continuous beep
  }
  else
  {
    status = "NORMAL";

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ================= SERIAL MONITOR =================
  Serial.println("----------------------------");

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Pitch: ");
  Serial.println(pitch);

  Serial.print("Roll: ");
  Serial.println(roll);

  Serial.print("AX: ");
  Serial.println(ax);

  Serial.print("AY: ");
  Serial.println(ay);

  Serial.print("AZ: ");
  Serial.println(az);

  Serial.print("GX: ");
  Serial.println(gx);

  Serial.print("GY: ");
  Serial.println(gy);

  Serial.print("GZ: ");
  Serial.println(gz);

  // ================= SCREEN 1 =================
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 16);
  display.print("Hum : ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 32);
  display.print("Pitch: ");
  display.print(pitch, 1);

  display.setCursor(0, 48);
  display.print("Roll : ");
  display.print(roll, 1);

  display.display();
  delay(5000);

  // ================= SCREEN 2 =================
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("SATELLITE HEALTH");

  display.setCursor(0, 20);
  display.print("STATUS:");

  display.setCursor(0, 40);
  display.print(status);

  display.display();
  delay(5000);

  // ================= SCREEN 3 =================
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("ACCELERATION");

  display.setCursor(0, 16);
  display.print("AX: ");
  display.println(ax);

  display.setCursor(0, 28);
  display.print("AY: ");
  display.println(ay);

  display.setCursor(0, 40);
  display.print("AZ: ");
  display.println(az);

  display.display();
  delay(5000);

  // ================= SCREEN 4 =================
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("GYROSCOPE");

  display.setCursor(0, 16);
  display.print("GX: ");
  display.println(gx);

  display.setCursor(0, 28);
  display.print("GY: ");
  display.println(gy);

  display.setCursor(0, 40);
  display.print("GZ: ");
  display.println(gz);

  display.display();
  delay(5000);

  // ================= THINGSPEAK =================
  if (WiFi.status() == WL_CONNECTED &&
      millis() - lastUpload >= 15000)
  {
    HTTPClient http;

    String url =
      "http://api.thingspeak.com/update?api_key=" + apiKey +
      "&field1=" + String(temperature) +
      "&field2=" + String(humidity) +
      "&field3=" + String(ax) +
      "&field4=" + String(ay) +
      "&field5=" + String(az) +
      "&field6=" + String(gx) +
      "&field7=" + String(gy) +
      "&field8=" + String(gz);

    http.begin(url);

    int httpCode = http.GET();

    Serial.print("ThingSpeak Response: ");
    Serial.println(httpCode);

    http.end();

    lastUpload = millis();
  }
}