#include <Arduino.h>
#include <Streaming.h>

// #include <Wire.h>
// #include <i2cdetect.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#include "hardware/Config.h"
#include "hardware/Display.h"
#include "hardware/SensorDHT22.h"

Display *display;
SensorDHT22 dht22Sensor(DHT22_PIN);

void setup()
{
  // Поиск адресов всех подключенных i2c устройств с помощью библиотеки i2cdetect
  // Wire.begin();
  // i2cdetect();


  Serial.begin(SERIAL_DEBUG_SPEED);
  Serial << "Start setup..." << endl;

  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  dht22Sensor.init();


  // Проверяем RTC
  Serial << "Checking RTC..." << endl;
  tmElements_t tm;
  if (RTC.read(tm)) {
    Serial << "Current time: " << ((tm.Hour < 10) ? "0" : "") << tm.Hour << ":" << ((tm.Minute < 10) ? "0" : "")
           << tm.Minute << ":" << ((tm.Second < 10) ? "0" : "") << tm.Second << " " << tmYearToCalendar(tm.Year)
           << "-" << ((tm.Month < 10) ? "0" : "") << tm.Month << "-" << ((tm.Day < 10) ? "0" : "") << tm.Day << endl;
  } else {
    Serial << "RTC fail!" << endl;
  }


  // Установка текущего времени
  // tm.Hour = 20;
  // tm.Minute = 11;
  // tm.Second = 0;
  // tm.Day = 12;
  // tm.Month = 8;
  // tm.Year = CalendarYrToTm(2020);
  // RTC.write(tm);


  display = new Display(LCD_ADDR, LCD_COLS, LCD_ROWS);

  // выключаем реле
  digitalWrite(RELAY_PIN, HIGH);

  Serial << "OK" << endl;
}

void loop()
{
  uint32_t now = millis();
  static uint32_t lastRefresh = 0;
  static uint32_t lastButtonClick = 0;
  static uint8_t lastHourSaveData = 200;

  static int8_t climatControlTemperature = INIT_CLIMAT_CONTROL_TEMPERATURE;

  static String hours[10] = {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "};
  static String temps[10] = {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "};
  static String humis[10] = {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "};

  static String onOff[12] = {"     ", "     ", "     ", "     ", "     ", "     ", "     ", "     ", "     ", "     ", "     ", "     "};
  static uint8_t currentOnOff = 0;

  static bool heaterIsOn = false;

  static bool showOnOffTimes = false;

  if (lastRefresh > now) {
    lastRefresh = 0;
  }

  if (lastButtonClick > now) {
    lastButtonClick = 0;
  }

  bool mustRefreshIfButtonClick = false;


  if ((now - lastButtonClick) > BUTTON_CLICK_INTERVAL) {
    tmElements_t tm;
    if (RTC.read(tm)) {
      String time = String((tm.Hour < 10) ? "0" : "") + String(tm.Hour) + String(":") + String((tm.Minute < 10) ? "0" : "") + String(tm.Minute);

      int buttonUp = digitalRead(BUTTON_UP_PIN);
      if (buttonUp == LOW) {
        climatControlTemperature++;
        if (heaterIsOn) {
          // выключаем реле
          digitalWrite(RELAY_PIN, HIGH);
          heaterIsOn = false;
          saveOnOffValue(currentOnOff, onOff, time);
        }
        mustRefreshIfButtonClick = true;
        lastButtonClick = now;
      }

      int buttonDown = digitalRead(BUTTON_DOWN_PIN);
      if (buttonDown == LOW) {
        climatControlTemperature--;
        if (heaterIsOn) {
          // выключаем реле
          digitalWrite(RELAY_PIN, HIGH);
          heaterIsOn = false;
          saveOnOffValue(currentOnOff, onOff, time);
        }
        mustRefreshIfButtonClick = true;
        lastButtonClick = now;
      }
    }
  }

  if (((now - lastRefresh) > REFRESH_INTERVAL) || mustRefreshIfButtonClick) {
    tmElements_t tm;
    if (RTC.read(tm)) {
      String time = String((tm.Hour < 10) ? "0" : "") + String(tm.Hour) + String(":") + String((tm.Minute < 10) ? "0" : "") + String(tm.Minute);

      int8_t temp = dht22Sensor.getTemp();
      int8_t humi = dht22Sensor.getHumi();
      refresh(lastHourSaveData, climatControlTemperature, hours, temps, humis, temp, humi, tm, onOff, showOnOffTimes);
      lastRefresh = now;

      if ((temp >= (climatControlTemperature + 1)) && heaterIsOn) {
        // выключаем реле
        digitalWrite(RELAY_PIN, HIGH);
        heaterIsOn = false;
        saveOnOffValue(currentOnOff, onOff, time);
      }

      if ((temp <= (climatControlTemperature - 1)) && !heaterIsOn) {
        // включаем реле
        digitalWrite(RELAY_PIN, LOW);
        heaterIsOn = true;
        saveOnOffValue(currentOnOff, onOff, time);
      }
    }
  }
}

void refresh(
  uint8_t &lastHourSaveData,
  int8_t climatControlTemperature,
  String hours[],
  String temps[],
  String humis[],
  int8_t temp,
  int8_t humi,
  tmElements_t tm,
  String onOff[],
  bool &showOnOffTimes
) {
  String time = String("00:00");
  String date = String("00/00");

  String tempStr = String(temp);
  String humiStr = String(humi);

  time = String((tm.Hour < 10) ? "0" : "") + String(tm.Hour) + String(":") + String((tm.Minute < 10) ? "0" : "") + String(tm.Minute);
  date = String((tm.Day < 10) ? "0" : "") + String(tm.Day) + String("/") + String((tm.Month < 10) ? "0" : "") + String(tm.Month);

  if ((tm.Hour % 2 == 0) && (tm.Hour != lastHourSaveData)) {
    for (uint8_t i = 0; i < 9; i++) {
      hours[i] = hours[i + 1];
      temps[i] = temps[i + 1];
      humis[i] = humis[i + 1];
    }
    hours[9] = String((tm.Hour < 10) ? "0" : "") + String(tm.Hour);

    String temp2Digits = tempStr.substring(0, 2);
    if (temp2Digits.length() == 1) {
      temp2Digits = String(" ") + temp2Digits;
    }

    String humi2Digits = humiStr.substring(0, 2);
    if (humi2Digits.length() == 1) {
      humi2Digits = String(" ") + humi2Digits;
    }

    temps[9] = temp2Digits;
    humis[9] = humi2Digits;

    lastHourSaveData = tm.Hour;
  }

  display->showText(time + String(" ") + date + " " + temp + String(" ") + humi + String(" ") + String(climatControlTemperature), 0);

  if (showOnOffTimes) {
    display->showText(onOff[0] + onOff[1] + onOff[2] + onOff[3], 1);
    display->showText(onOff[4] + onOff[5] + onOff[6] + onOff[7], 2);
    display->showText(onOff[8] + onOff[9] + onOff[10] + onOff[11], 3);
  } else {
    String hoursString = String("");
    String tempsString = String("");
    String humisString = String("");

    for (uint8_t i = 0; i < 10; i++) {
      hoursString += hours[i];
      tempsString += temps[i];
      humisString += humis[i];
    }
    display->showText(hoursString, 1);
    display->showText(tempsString, 2);
    display->showText(humisString, 3);
  }
  showOnOffTimes = !showOnOffTimes;
}

void saveOnOffValue(uint8_t &currentOnOff, String onOff[], String value)
{
  if (currentOnOff < 12) {
    onOff[currentOnOff] = value;
    currentOnOff++;
  } else {
    for (uint8_t i = 0; i < 10; i++) {
      onOff[i] = onOff[i + 2];
    }
    onOff[10] = value;
    onOff[11] = "     ";
    currentOnOff = 11;
  }
}
