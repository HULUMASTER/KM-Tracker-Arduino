#ifndef SD_CARD_HANDLER_H
#define SD_CARD_HANDLER_H

#include <SD.h>

class SDCardHandler {
private:
  File masterFile;
  File userFiles[NUM_USERS];
  
public:
  bool begin() {
    if (!SD.begin(SD_CS_PIN)) {
      DEBUG_PRINTLN("[SD] Initialisierung fehlgeschlagen!");
      return false;
    }
    
    DEBUG_PRINTLN("[SD] Karte erkannt!");
    createHeadersIfNeeded();
    return true;
  }
  
  void createHeadersIfNeeded() {
    // Master CSV
    if (!SD.exists("master.csv")) {
      File f = SD.open("master.csv", FILE_WRITE);
      f.println("Timestamp,User,Latitude,Longitude,Speed_kmh,Distance_km");
      f.close();
    }
    
    // User CSVs
    for (int i = 0; i < NUM_USERS; i++) {
      String filename = String(USER_NAMES[i]) + ".csv";
      if (!SD.exists(filename.c_str())) {
        File f = SD.open(filename.c_str(), FILE_WRITE);
        f.println("Timestamp,Latitude,Longitude,Speed_kmh,Distance_km,Cumulative_km");
        f.close();
      }
    }
  }
  
  void saveRecord(unsigned long timestamp, double lat, double lon,
                  double speed, double distance, User user, 
                  double cumulativeKM) {
    
    // Master CSV
    File f = SD.open("master.csv", FILE_WRITE);
    if (f) {
      f.print(timestamp); f.print(",");
      f.print(USER_NAMES[user]); f.print(",");
      f.print(lat, 6); f.print(",");
      f.print(lon, 6); f.print(",");
      f.print(speed, 2); f.print(",");
      f.println(distance, 6);
      f.close();
    }
    
    // User CSV
    String filename = String(USER_NAMES[user]) + ".csv";
    f = SD.open(filename.c_str(), FILE_WRITE);
    if (f) {
      f.print(timestamp); f.print(",");
      f.print(lat, 6); f.print(",");
      f.print(lon, 6); f.print(",");
      f.print(speed, 2); f.print(",");
      f.print(distance, 6); f.print(",");
      f.println(cumulativeKM, 2);
      f.close();
    }
    
    DEBUG_PRINT("[SD] Gespeichert: ");
    DEBUG_PRINT(USER_NAMES[user]);
    DEBUG_PRINT(" - ");
    DEBUG_PRINT(cumulativeKM, 2);
    DEBUG_PRINTLN(" km");
  }
  
  double loadCumulativeKM(User user) {
    String filename = String(USER_NAMES[user]) + ".csv";
    
    if (!SD.exists(filename.c_str())) {
      return 0.0;
    }
    
    File f = SD.open(filename.c_str());
    double lastCumulative = 0.0;
    
    // Skip header
    f.readStringUntil('\n');
    
    // Zu letzter Zeile springen
    while (f.available()) {
      String line = f.readStringUntil('\n');
      int lastComma = line.lastIndexOf(',');
      if (lastComma > 0) {
        String kmStr = line.substring(lastComma + 1);
        lastCumulative = kmStr.toDouble();
      }
    }
    
    f.close();
    return lastCumulative;
  }
};

#endif
