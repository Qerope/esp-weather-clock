#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>

#include "config.h"

Adafruit_SSD1306 display(-1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", TZ_OFFSET, 60000);

const unsigned long WEATHER_INTERVAL = 600000UL;
unsigned long lastWeather = 0;
float temperature = NAN;
String condition = "";
unsigned long lastTick = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(WiFi.localIP());
  display.display();
  timeClient.begin();
}

bool fetchWeather() {
  HTTPClient http;
  http.begin(String("http://api.openweathermap.org/data/2.5/weather?q=") + OWM_CITY +
             "&units=metric&appid=" + OWM_KEY);
  int code = http.GET();
  if (code != 200) {
    http.end();
    return false;
  }
  String body = http.getString();
  http.end();
  DynamicJsonBuffer buf;
  JsonObject& root = buf.parseObject(body);
  if (!root.success()) {
    return false;
  }
  temperature = root["main"]["temp"];
  condition = root["weather"][0]["main"].as<String>();
  return true;
}

void drawClock() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(4, 4);
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
  display.print(buf);
  display.setTextSize(1);
  display.setCursor(4, 44);
  if (!isnan(temperature)) {
    display.print(temperature, 1);
    display.print(" C  ");
    display.print(condition);
  }
  display.display();
}

void loop() {
  timeClient.update();
  if (lastWeather == 0 || millis() - lastWeather > WEATHER_INTERVAL) {
    if (fetchWeather()) {
      lastWeather = millis();
    } else {
      lastWeather = millis() - WEATHER_INTERVAL + 60000UL;
    }
  }
  if (millis() - lastTick >= 1000) {
    lastTick = millis();
    drawClock();
  }
  yield();
}
