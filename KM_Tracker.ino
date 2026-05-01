/*
  ========================================
  KM-TRACKER FÜR ARDUINO UNO R4 WIFI
  ========================================
  
  Hardware:
  - Arduino Uno R4 WiFi
  - Neo-M8N GPS (TX0, RX1 - Serial1)
  - SSD1306 OLED 128x64 (I2C @ 0x3C)
  - SD-Karte (SPI, CS=10)
  - 2x Buttons (Pin 2, 3)
  - Buzzer (Pin 4)
  
  Autor: HULUMASTER | 2026
*/

#include "config.h"
#include "GPSHandler.h"
#include "DataBuffer.h"
#include "SDCardHandler.h"
#include "OLEDDisplay.h"
#include "ButtonHandler.h"
#include "BuzzerHandler.h"

// ===== GLOBALE OBJEKTE =====
GPSHandler gps;
DataBuffer dataBuffer;
SDCardHandler sdCard;
OLEDDisplay display;
ButtonHandler buttons;
BuzzerHandler buzzer;

// ===== ZUSTANDSVARIABLEN =====
User currentUser = LEVI;
unsigned long lastGPSReadTime = 0;
unsigned long lastSaveTime = 0;
bool userSelectionMode = false;
double currentCumulativeKM = 0.0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n╔════ KM-TRACKER START ════╗");
  
  buttons.begin();
  buzzer.begin();
  
  if (!display.begin()) {
    Serial.println("OLED Error!");
    while (1);
  }
  
  if (!sdCard.begin()) {
    Serial.println("SD Card Error!");
    while (1);
  }
  
  gps.begin();
  loadAllUserData();
  
  Serial.println("╚═════════════════════════╝\n");
}

void loop() {
  // Button-Eingaben verarbeiten
  if (buttons.isUserButtonPressed()) {
    handleUserButtonPress();
  }
  
  if (buttons.isConfirmButtonPressed()) {
    handleConfirmButtonPress();
  }
  
  // GPS Daten alle 2 Sekunden lesen
  if (millis() - lastGPSReadTime > GPS_READ_INTERVAL) {
    if (gps.readGPS()) {
      processGPSData();
    }
    lastGPSReadTime = millis();
  }
  
  // Display aktualisieren
  if (!userSelectionMode) {
    display.showMainScreen(currentUser, gps.getSpeed(), 
                          currentCumulativeKM, gps.getSatellites(), 
                          gps.getIsMoving());
  }
  
  // Daten alle 10 Sekunden speichern
  if (millis() - lastSaveTime > SAVE_INTERVAL) {
    if (dataBuffer.getBufferSize() > 0) {
      saveBufferToSD();
    }
    lastSaveTime = millis();
  }
}

void processGPSData() {
  if (gps.getSatellites() < 4) {
    display.showGPSSearching();
    return;
  }
  
  double distanceDelta = gps.getDistanceDelta();
  
  if (distanceDelta > 0.0) {
    currentCumulativeKM += distanceDelta;
    
    dataBuffer.addRecord(
      (unsigned long)millis() / 1000,
      gps.getLat(),
      gps.getLon(),
      gps.getSpeed(),
      distanceDelta,
      currentUser
    );
    
    if (currentUser == OTHER && gps.getIsMoving()) {
      buzzer.warnDrivingWithoutUser();
    }
  }
}

void handleUserButtonPress() {
  if (!userSelectionMode) {
    userSelectionMode = true;
  } else {
    currentUser = (User)((currentUser + 1) % NUM_USERS);
    buzzer.confirmUserSelection();
  }
  
  display.showUserSelection(currentUser);
}

void handleConfirmButtonPress() {
  if (userSelectionMode) {
    currentCumulativeKM = dataBuffer.getTotalDistance(currentUser);
    userSelectionMode = false;
    buzzer.confirmUserSelection();
  }
}

void saveBufferToSD() {
  GPSRecord* buffer = dataBuffer.getBuffer();
  int size = dataBuffer.getBufferSize();
  
  for (int i = 0; i < size; i++) {
    sdCard.saveRecord(
      buffer[i].timestamp,
      buffer[i].lat,
      buffer[i].lon,
      buffer[i].speed,
      buffer[i].distance,
      buffer[i].user,
      dataBuffer.getTotalDistance(buffer[i].user)
    );
  }
  
  buzzer.confirmSave();
  dataBuffer.clearBuffer();
}

void loadAllUserData() {
  for (int i = 0; i < NUM_USERS; i++) {
    User user = (User)i;
    double loadedKM = sdCard.loadCumulativeKM(user);
    dataBuffer.loadFromSD(user, loadedKM);
  }
  
  currentCumulativeKM = dataBuffer.getTotalDistance(currentUser);
}
