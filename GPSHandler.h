#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <TinyGPS++.h>

class GPSHandler {
private:
  TinyGPSPlus gps;
  HardwareSerial* gpsSerial;
  
  // Letzte bekannte Position
  double lastLat = 0.0;
  double lastLon = 0.0;
  double totalDistance = 0.0;
  double currentSpeed = 0.0;
  int satellites = 0;
  bool isMoving = false;
  unsigned long lastMoveTime = 0;
  
  // Puffer für smoothing
  double speedBuffer[5] = {0, 0, 0, 0, 0};
  int speedIndex = 0;
  
public:
  GPSHandler() : gpsSerial(&Serial1) {}
  
  void begin() {
    gpsSerial->begin(GPS_BAUD);
    DEBUG_PRINTLN("[GPS] Initialisiert @ 9600 Baud");
  }
  
  bool readGPS() {
    while (gpsSerial->available() > 0) {
      if (gps.encode(gpsSerial->read())) {
        if (gps.location.isValid() && gps.satellites.isValid()) {
          processLocation();
          return true;
        }
      }
    }
    return false;
  }
  
  void processLocation() {
    satellites = gps.satellites.value();
    
    // Speed smoothing (Durchschnitt der letzten 5 Werte)
    currentSpeed = gps.speed.kmph();
    speedBuffer[speedIndex] = currentSpeed;
    speedIndex = (speedIndex + 1) % 5;
    
    double avgSpeed = 0;
    for (int i = 0; i < 5; i++) avgSpeed += speedBuffer[i];
    avgSpeed /= 5.0;
    
    isMoving = avgSpeed > MIN_SPEED_THRESHOLD;
    
    if (isMoving) {
      lastMoveTime = millis();
    }
    
    // Distanz berechnen (nur wenn bewegung existiert)
    if (lastLat != 0.0 && lastLon != 0.0) {
      totalDistance += haversineDistance(
        lastLat, lastLon,
        gps.location.lat(), gps.location.lng()
      );
    }
    
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();
  }
  
  // ===== HAVERSINE FORMEL (SEHR GENAU!) =====
  double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dlat = DEG_TO_RAD(lat2 - lat1);
    double dlon = DEG_TO_RAD(lon2 - lon1);
    
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(DEG_TO_RAD(lat1)) * cos(DEG_TO_RAD(lat2)) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c;
  }
  
  // ===== GETTER =====
  double getLat() const { return lastLat; }
  double getLon() const { return lastLon; }
  double getSpeed() const { return currentSpeed; }
  double getTotalDistance() const { return totalDistance; }
  int getSatellites() const { return satellites; }
  bool getIsMoving() const { return isMoving; }
  
  double getDistanceDelta() {
    if (isMoving && lastLat != 0.0) {
      double delta = totalDistance;
      totalDistance = 0.0;
      return delta;
    }
    return 0.0;
  }
};

#endif
