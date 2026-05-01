#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

struct GPSRecord {
  unsigned long timestamp;
  double lat;
  double lon;
  double speed;
  double distance;
  User user;
};

class DataBuffer {
private:
  GPSRecord records[MAX_BUFFER_SIZE];
  int bufferIndex = 0;
  double userCumulativeKM[NUM_USERS] = {0, 0, 0};
  
public:
  void addRecord(unsigned long timestamp, double lat, double lon, 
                 double speed, double distance, User user) {
    
    if (bufferIndex < MAX_BUFFER_SIZE) {
      records[bufferIndex].timestamp = timestamp;
      records[bufferIndex].lat = lat;
      records[bufferIndex].lon = lon;
      records[bufferIndex].speed = speed;
      records[bufferIndex].distance = distance;
      records[bufferIndex].user = user;
      
      userCumulativeKM[user] += distance;
      bufferIndex++;
    }
  }
  
  GPSRecord* getBuffer() {
    return records;
  }
  
  int getBufferSize() const {
    return bufferIndex;
  }
  
  void clearBuffer() {
    bufferIndex = 0;
  }
  
  double getTotalDistance(User user) const {
    return userCumulativeKM[user];
  }
  
  void loadFromSD(User user, double km) {
    userCumulativeKM[user] = km;
  }
  
  void printStatus() {
    Serial.println("╔════ BUFFER STATUS ════╗");
    for (int i = 0; i < NUM_USERS; i++) {
      Serial.print(USER_NAMES[i]);
      Serial.print(": ");
      Serial.print(userCumulativeKM[i], 2);
      Serial.println(" km");
    }
    Serial.println("╚═══════════════════════╝");
  }
};

#endif
