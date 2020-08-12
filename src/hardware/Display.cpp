#include "Display.h"
#include <Streaming.h>

// Делим экран на сегменты, длина левого блока (символы)
#define LEFT_SIGNS_COUNT 12

// Делим экран на сегменты, длина правого блока (символы)
#define RIGHT_SIGNS_COUNT 8

// Длина строки (символы)
#define STRING_SIGNS_COUNT 20

// Максимальная длина текста на экране
#define TEXT_SIGNS_COUNT 80

// Пустая строка (заполнена пробелами)
#define EMPTY_ROW "                    "

// Реально пустая строка
#define NULL_STRING ""

Display::Display(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
  _lcd = new LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows);
  _lcd->init();
  _lcd->backlight();
  _lcd->clear();
}

void Display::showLeftText(String text, uint8_t row)
{
  String newStr = text.substring(0, LEFT_SIGNS_COUNT);
  if (_leftTexts[row] != newStr) {
    // TODO тут можно попробовать сделать оптимальнее. Сейчас мы
    // 2 раза обновляем строку - сначала ее очищаем, а потом уже поверх
    // выводим нужные данные.
    // Нужно переделать так: берем строку для вывода, дополняем ее до
    // длины LEFT_SIGNS_COUNT и просто выводим.
    // Это пожно сделать когда будет время и пульт под рукой
    _lcd->setCursor(0, row);
    _lcd->print(String(EMPTY_ROW).substring(0, LEFT_SIGNS_COUNT));
    _lcd->setCursor(0, row);
    _lcd->print(newStr);
    _leftTexts[row] = newStr;
  }
}

void Display::clearLeftText(uint8_t row)
{
  showLeftText(NULL_STRING, row);
}

void Display::showRightText(String text, uint8_t row)
{
  String newStr = text.substring(0, RIGHT_SIGNS_COUNT);
  if (_rightTexts[row] != newStr) {
    // TODO тут можно попробовать сделать оптимальнее. Сейчас мы
    // 2 раза обновляем строку - сначала ее очищаем, а потом уже поверх
    // выводим нужные данные.
    // Нужно переделать так: берем строку для вывода, дополняем ее до
    // длины RIGHT_SIGNS_COUNT и просто выводим.
    // Это пожно сделать когда будет время и пульт под рукой
    _lcd->setCursor(LEFT_SIGNS_COUNT, row);
    _lcd->print(String(EMPTY_ROW).substring(0, RIGHT_SIGNS_COUNT));
    _lcd->setCursor(LEFT_SIGNS_COUNT, row);
    _lcd->print(newStr);
    _rightTexts[row] = newStr;
  }
}

void Display::clearRightText(uint8_t row)
{
  showRightText(NULL_STRING, row);
}

void Display::showText(String text, uint8_t row)
{
  uint16_t length = text.length();

  if (length > STRING_SIGNS_COUNT) {
    text = text.substring(0, STRING_SIGNS_COUNT);
    length = STRING_SIGNS_COUNT;
  }

  String leftStr = text.substring(0, min(LEFT_SIGNS_COUNT, length));
  String rightStr;
  if (length > LEFT_SIGNS_COUNT) {
    rightStr = text.substring(LEFT_SIGNS_COUNT);
  } else {
    rightStr = NULL_STRING;
  }
  showLeftText(leftStr, row);
  showRightText(rightStr, row);
}

void Display::showFullScreenText(String text)
{
  uint16_t length = text.length();

  if (length > TEXT_SIGNS_COUNT) {
    text = text.substring(0, TEXT_SIGNS_COUNT);
    length = TEXT_SIGNS_COUNT;
  }

  uint8_t fromPos = 0;
  uint8_t row = 0;

  while (fromPos < length) {
    uint8_t nextFromPos = fromPos + STRING_SIGNS_COUNT;
    showText(text.substring(fromPos, min(nextFromPos, length)), row);
    fromPos = nextFromPos;
    row++;
  }

  while (row < 4) {
    showText(NULL_STRING, row);
    row++;
  }
}

void Display::clearExceptFirstLeft()
{
  showRightText(NULL_STRING, 0);
  showLeftText(NULL_STRING, 1);
  showRightText(NULL_STRING, 1);
  showLeftText(NULL_STRING, 2);
  showRightText(NULL_STRING, 2);
  showLeftText(NULL_STRING, 3);
  showRightText(NULL_STRING, 3);
}

void Display::showModalText(String text)
{
  if (text.length() == 0) {
    return;
  }

  _lcd->setCursor(0, 0);

  if (text.length() > TEXT_SIGNS_COUNT) {
    text = text.substring(0, TEXT_SIGNS_COUNT);
  } else if (text.length() < TEXT_SIGNS_COUNT) {
    _lcd->clear();
  }
  _lcd->print(text);
}

void Display::recoverRealText()
{
  _lcd->setCursor(0, 0);
  _lcd->clear();
  for (uint8_t row = 0; row < 4; row++) {
    _lcd->setCursor(0, row);
    _lcd->print(_leftTexts[row]);
    _lcd->setCursor(0, row);
    _lcd->setCursor(LEFT_SIGNS_COUNT, row);
    _lcd->print(_rightTexts[row]);
  }
}