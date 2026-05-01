#ifndef BUZZER_HANDLER_H
#define BUZZER_HANDLER_H

class BuzzerHandler {
public:
  void begin() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  void confirmUserSelection() {
    tone(BUZZER_PIN, 1000, 200);  // 1kHz für 200ms
    delay(100);
    tone(BUZZER_PIN, 1200, 200);  // 1.2kHz für 200ms
  }
  
  void confirmSave() {
    tone(BUZZER_PIN, 800, 150);
  }
  
  void errorSound() {
    tone(BUZZER_PIN, 400, 500);  // 400Hz für 500ms
  }
  
  void warnDrivingWithoutUser() {
    tone(BUZZER_PIN, 600, 100);
    delay(100);
    tone(BUZZER_PIN, 600, 100);
  }
};

#endif
