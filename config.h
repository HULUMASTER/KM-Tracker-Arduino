#ifndef CONFIG_H
#define CONFIG_H

// ===== DEBUG =====
#define DEBUG_MODE true
#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// ===== BENUTZER =====
enum User { LEVI = 0, BASIL = 1, OTHER = 2 };
const int NUM_USERS = 3;
const char* USER_NAMES[] = {"LEVI", "BASIL", "OTHER"};

// ===== PIN DEFINITIONEN =====
#define GPS_RX_PIN 1      // Serial1
#define GPS_TX_PIN 0      // Serial1
#define BUTTON_USER_PIN 2
#define BUTTON_CONFIRM_PIN 3
#define BUZZER_PIN 4
#define SD_CS_PIN 10
#define OLED_SCL_PIN 19   // I2C
#define OLED_SDA_PIN 18   // I2C

// ===== GPS EINSTELLUNGEN =====
#define GPS_BAUD 9600
#define GPS_READ_INTERVAL 2000      // 2 Sekunden
#define MIN_SPEED_THRESHOLD 1.0     // km/h
#define STILLSTAND_TIMEOUT 30000    // 30 Sekunden

// ===== SPEICHER EINSTELLUNGEN =====
#define SAVE_INTERVAL 10000         // 10 Sekunden
#define MAX_BUFFER_SIZE 50          // Max Records im RAM

// ===== OLED EINSTELLUNGEN =====
#define OLED_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_UPDATE_INTERVAL 500    // 0.5 Sekunden

// ===== HAVERSINE KONSTANTEN (sehr genaue Berechnung) =====
#define EARTH_RADIUS_KM 6371.0
#define DEG_TO_RAD(x) ((x) * 3.14159265359 / 180.0)

#endif
