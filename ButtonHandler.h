#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

class ButtonHandler {
private:
  unsigned long lastUserButtonPress = 0;
  unsigned long lastConfirmButtonPress = 0;
  const int DEBOUNCE_DELAY = 250;  // 250ms
  
public:
  void begin() {
    pinMode(BUTTON_USER_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);
  }
  
  bool isUserButtonPressed() {
    if (digitalRead(BUTTON_USER_PIN) == LOW) {
      if (millis() - lastUserButtonPress > DEBOUNCE_DELAY) {
        lastUserButtonPress = millis();
        return true;
      }
    }
    return false;
  }
  
  bool isConfirmButtonPressed() {
    if (digitalRead(BUTTON_CONFIRM_PIN) == LOW) {
      if (millis() - lastConfirmButtonPress > DEBOUNCE_DELAY) {
        lastConfirmButtonPress = millis();
        return true;
      }
    }
    return false;
  }
};

#endif
