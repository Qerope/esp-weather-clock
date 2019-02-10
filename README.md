# esp-weather-clock

NodeMCU (ESP8266) + SSD1306 128x64 I2C OLED. Shows the time, current
temperature and conditions from OpenWeatherMap.

Wiring: OLED SDA -> D2, SCL -> D1, 3.3V, GND.

Setup:

1. Copy `config.h.example` to `config.h` and fill it in.
2. Install ESP8266 board package 2.4.x and the libraries ArduinoJson 5.13,
   NTPClient, Adafruit SSD1306 and Adafruit GFX.
3. Upload. After the first flash you can update over the air (`esp-clock`
   shows up as a network port in the Arduino IDE).

The display dims between 23:00 and 06:00.
