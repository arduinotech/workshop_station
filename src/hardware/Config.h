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

#define SERIAL_DEBUG_SPEED 115200

#define REFRESH_INTERVAL 10000
#define BUTTON_CLICK_INTERVAL 500

#define INIT_CLIMAT_CONTROL_TEMPERATURE 5

#endif