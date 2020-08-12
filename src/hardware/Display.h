#ifndef Display_h_
#define Display_h_

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Display {
  public:
    /**
     * @brief Инициализация, включение и очистка дисплея
     *
     * @param lcd_addr адрес дисплея
     * @param lcd_cols кол-во символов в строке
     * @param lcd_rows кол-во строк
     */
    Display(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
    /** LEFT_SIGNS_COUNT signs */

    /**
     * @brief Вывод строки в левую часть,
     * максимальная длина строки - LEFT_SIGNS_COUNT
     *
     * @param text текст для вывода
     * @param row номер строки, в какой показывать текст
     */
    void showLeftText(String text, uint8_t row);

    /**
     * @brief Чистим определенную строку левого сегмента
     *
     * @param row
     */
    void clearLeftText(uint8_t row);

    /**
     * @brief Вывод строки в левую часть,
     * максимальная длина строки - RIGHT_SIGNS_COUNT
     *
     * @param text текст для вывода
     * @param row номер строки, в какой показывать текст
     */
    void showRightText(String text, uint8_t row);

    /**
     * @brief Чистим определенную строку правого сегмента
     *
     * @param row
     */
    void clearRightText(uint8_t row);

    /**
     * @brief Показываем текст во всю строку,
     * максимальная длина строки - STRING_SIGNS_COUNT
     *
     * @param text
     * @param row
     */
    void showText(String text, uint8_t row);

    /**
     * @brief Показываем текст на весь экран,
     * максимальная длина текста - TEXT_SIGNS_COUNT
     *
     * @param text
     */
    void showFullScreenText(String text);

    /**
     * @brief Показываем текст на весь экран, но при этом сам текст в
     * _leftTexts и _rightTexts не сохраняем и никак не проверяем, что сейчас
     * на экране. Это аналог модельного окна поверх реального. После показа
     * реальный текст будет восстановлен по функции recoverRealText().
     *
     * @param text
     */
    void showModalText(String text);

    /**
     * @brief Восстанавливает на экране реальный текст после вызова
     * showFullScreenText(), когда нужно будет убрать временный.
     */
    void recoverRealText();

    /**
     * @brief Очистка всего экрана кроме левой верхней части
     *
     */
    void clearExceptFirstLeft();

  private:
    LiquidCrystal_I2C *_lcd;
    String _leftTexts[4];
    String _rightTexts[4];
};

#endif