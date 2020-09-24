#ifndef Config_h_
#define Config_h_

// =================================
// ============= PINS ==============
// =================================

#define DHT22_PIN 2
#define SPEAKER_PIN 3
#define RELAY_PIN 5
#define BUTTON_DOWN_PIN 7
#define BUTTON_UP_PIN 8

// =================================
// ============= OTHERS ============
// =================================

// Дисплей
#define LCD_ADDR 0x3f
#define LCD_COLS 20
#define LCD_ROWS 4

// скорость сериального порта для
// вывода отладочной информации
#define SERIAL_DEBUG_SPEED 115200

// интервал обновления экрана в миллисекундах
#define REFRESH_INTERVAL 10000

// время задержки между нажатиями кнопок
// для борьбы с двойным сраьбатыванием в
// миллисекундах
#define BUTTON_CLICK_INTERVAL 500

// значение температуры для климат-контроля
// по-умолчанию (после включения)
#define INIT_CLIMAT_CONTROL_TEMPERATURE 5

#endif