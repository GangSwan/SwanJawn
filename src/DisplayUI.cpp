#include "DisplayUI.h"
#include "FileBrowser.h"

namespace {
  // Local definitions for menu texts with new features:
  // Updated main menu now has 5 items including the file browser.
  const char* mainMenu[] = { "Sniffing", "Info Mode", "Silent Mode", "Buzzer Songs", "File Browser" };
  const int mainMenuLength = sizeof(mainMenu) / sizeof(mainMenu[0]);
  
  const char* sniffMenu[] = { "Packet Sniffing", "Access Points", "Back" };
  const int sniffMenuLength = sizeof(sniffMenu) / sizeof(sniffMenu[0]);
  
  const char* buzzerMenu[] = { "Giorno's Theme", "Back" };
  const int buzzerMenuLength = sizeof(buzzerMenu) / sizeof(buzzerMenu[0]);
  
  const char* subMenu[] = { "Do nothing", "Back" };
  const int subMenuLength = sizeof(subMenu) / sizeof(subMenu[0]);
}

namespace DisplayUI {
  // Initialize our global UI state variables
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
    
    switch(menuState) {
      case MAIN_MENU:
        display.println("Main Menu:");
        for (int i = 0; i < mainMenuLength; i++) {
          display.print((i == mainSelection) ? "> " : "  ");
          display.println(mainMenu[i]);
        }
        break;
        
      case INFO_MODE:
        display.println("Info Mode:");
        // (You can add additional system stats here)
        display.println("System Stats...");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
        
      case SILENT_MODE:
        display.println("Silent Mode:");
        // (Customize the UI for silent mode if needed)
        display.println("Buzzer/LEDs Off");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
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
        display.println("Packet Sniffing:");
        for (int i = 0; i < 30; i++) {
          int x = random(0, SCREEN_WIDTH);
          int y = random(0, SCREEN_HEIGHT - 10);
          display.drawPixel(x, y, SSD1306_WHITE);
        }
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
        
      case SNIFF_AP:
        display.println("Access Points:");
        display.println("Not yet implemented");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
        
      case FILE_BROWSER:
      // Delegate update to the FileBrowser module based on its internal state.
      if (FileBrowser::browserState == FileBrowser::FILE_LIST) {
        FileBrowser::updateFileListDisplay(display);
      } else if (FileBrowser::browserState == FileBrowser::FILE_VIEW) {
        FileBrowser::updateFileViewerDisplay(display);
      }
      break;
      
    }
    
    display.display();
  }
}
