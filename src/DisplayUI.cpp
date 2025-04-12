#include "DisplayUI.h"

namespace {
  // Local definitions for menu texts with new features:
  const char* mainMenu[] = { "Sniffing", "Info Mode", "Silent Mode", "Buzzer Songs" };
  const int mainMenuLength = sizeof(mainMenu) / sizeof(mainMenu[0]);
  
  const char* sniffMenu[] = { "Packet Sniffing", "Access Points", "Back" };
  const int sniffMenuLength = sizeof(sniffMenu) / sizeof(sniffMenu[0]);
  
  const char* buzzerMenu[] = { "Giorno's Theme", "Back" };
  const int buzzerMenuLength = sizeof(buzzerMenu) / sizeof(buzzerMenu[0]);
  
  const char* subMenu[] = { "Do nothing", "Back" };
  const int subMenuLength = sizeof(subMenu) / sizeof(subMenu[0]);
}

namespace DisplayUI {
  MenuState menuState = MAIN_MENU;
  int mainSelection = 0;
  int subSelection = 0;
  int sniffSelection = 0;
  
  void begin(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
  }
  
  void update(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
    
    // Render UI based on the current state:
    switch(menuState) {
      case MAIN_MENU:
        display.println("Main Menu:");
        for (int i = 0; i < mainMenuLength; i++) {
          display.print((i == mainSelection) ? "> " : "  ");
          display.println(mainMenu[i]);
        }
        break;
        
      case BUZZER_MENU:
        display.println("Buzzer Songs:");
        for (int i = 0; i < buzzerMenuLength; i++) {
          display.print((i == subSelection) ? "> " : "  ");
          display.println(buzzerMenu[i]);
        }
        break;
        
      case SUB_MENU:
        display.println("Submenu:");
        for (int i = 0; i < subMenuLength; i++) {
          display.print((i == subSelection) ? "> " : "  ");
          display.println(subMenu[i]);
        }
        break;
        
      case SNIFF_MENU:
        display.println("Sniffing Menu:");
        for (int i = 0; i < sniffMenuLength; i++) {
          display.print((i == sniffSelection) ? "> " : "  ");
          display.println(sniffMenu[i]);
        }
        break;
        
      case SNIFF_PACKET:
        // Display a fake real-time graph for packet sniffing
        display.println("Packet Sniffing:");
        for (int i = 0; i < 30; i++) {
          int x = random(0, SCREEN_WIDTH);
          int y = random(0, SCREEN_HEIGHT - 10); // Reserve space for the "Back" option
          display.drawPixel(x, y, SSD1306_WHITE);
        }
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
        
      case SNIFF_AP:
        // Placeholder for Access Points functionality
        display.println("Access Points:");
        display.println("Not yet implemented");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
    }
    
    display.display();
  }
}
