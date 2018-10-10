#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "your-ssid";
const char* password = "your-password";
Adafruit_SSD1306 display(-1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
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

void drawClock() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(4, 4);
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d:%02d", timeClient.getHours(), timeClient.getMinutes());
  display.print(buf);
  display.display();
}

void loop() {
  timeClient.update();
    fetchWeather();
    lastWeather = millis();
  drawClock();
  delay(1000);
  yield();
}
