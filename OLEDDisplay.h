#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

class OLEDDisplay {
private:
  Adafruit_SSD1306 display;
  unsigned long lastUpdateTime = 0;
  
public:
  OLEDDisplay() : display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1) {}
  
  bool begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
      return false;
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("KM-TRACKER");
    display.println("Starting...");
    display.display();
    
    return true;
  }
  
  void showMainScreen(User user, double speed, double cumulativeKM, 
                      int satellites, bool isMoving) {
    if (millis() - lastUpdateTime < OLED_UPDATE_INTERVAL) return;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // User
    display.setCursor(0, 0);
    display.print("User: ");
    display.println(USER_NAMES[user]);
    
    // Speed
    display.setCursor(0, 10);
    display.print("Speed: ");
    display.print(speed, 1);
    display.println(" km/h");
    
    // Distance
    display.setCursor(0, 20);
    display.print("Distance: ");
    display.print(cumulativeKM, 2);
    display.println(" km");
    
    // Status
    display.setCursor(0, 30);
    display.print("Status: ");
    display.println(isMoving ? "DRIVING" : "IDLE");
    
    // GPS
    display.setCursor(0, 40);
    display.print("GPS Sats: ");
    display.println(satellites);
    
    // Instructions
    display.setCursor(0, 55);
    display.setTextSize(1);
    display.println("BTN1: User  BTN2: OK");
    
    display.display();
    lastUpdateTime = millis();
  }
  
  void showUserSelection(User currentUser) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("SELECT USER");
    
    display.setTextSize(1);
    display.setCursor(0, 20);
    
    for (int i = 0; i < NUM_USERS; i++) {
      if (i == currentUser) {
        display.println("> " + String(USER_NAMES[i]));
      } else {
        display.println("  " + String(USER_NAMES[i]));
      }
    }
    
    display.setCursor(0, 55);
    display.println("BTN2: Confirm");
    display.display();
  }
  
  void showGPSSearching() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.println("GPS...");
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("Searching for satellites");
    display.display();
  }
  
  void showError(const char* error) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("ERROR!");
    display.println(error);
    display.display();
  }
  
  void showSavingScreen(User user, double distance) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Saving...");
    display.print(USER_NAMES[user]);
    display.print(" +");
    display.print(distance, 3);
    display.println(" km");
    display.display();
  }
};

#endif
