#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define BUZZER_PIN 18

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// WiFi Credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak Write API Key
String apiKey = "YQTTOS3Y6T644HZ2";

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  Wire.begin(21, 22);

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.display();

  // Connect WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  Serial.println("Satellite Health Monitoring Started");
}

void uploadToThingSpeak(float temp, float pitch, float roll)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    String url =
      "http://api.thingspeak.com/update?api_key=" +
      apiKey +
      "&field1=" + String(temp) +
      "&field2=" + String(pitch) +
      "&field3=" + String(roll);

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode > 0)
    {
      Serial.println("Data Uploaded to ThingSpeak");
    }
    else
    {
      Serial.println("Upload Failed");
    }

    http.end();
  }
}

void loop() {

  float temp = dht.readTemperature();

  sensors_event_t a, g, tempSensor;
  mpu.getEvent(&a, &g, &tempSensor);

  float pitch = atan2(a.acceleration.y,
                      sqrt(a.acceleration.x * a.acceleration.x +
                           a.acceleration.z * a.acceleration.z))
                * 180.0 / PI;

  float roll = atan2(-a.acceleration.x,
                     a.acceleration.z)
               * 180.0 / PI;

  String status = "HEALTHY";

  if (temp > 50 || abs(pitch) > 20 || abs(roll) > 20)
  {
    status = "WARNING";
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.println("-------------------");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Pitch: ");
  Serial.println(pitch);

  Serial.print("Roll: ");
  Serial.println(roll);

  Serial.print("Status: ");
  Serial.println(status);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 15);
  display.print("Pitch: ");
  display.print(pitch, 1);

  display.setCursor(0, 30);
  display.print("Roll: ");
  display.print(roll, 1);

  display.setCursor(0, 50);
  display.print("Status: ");
  display.print(status);

  display.display();

  // Upload to ThingSpeak
  uploadToThingSpeak(temp, pitch, roll);

  // ThingSpeak minimum update interval
  delay(15000);
}