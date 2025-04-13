#include "DisplayUI.h"
#include "FileBrowser.h"
#include "Sniffer.h"

namespace {
  // Local definitions for menu texts with new features:
  // Updated main menu now has 5 items including the file browser.
  const char* mainMenu[] = { "Sniffing", "Info Mode", "Silent Mode", "Buzzer Songs", "File Browser" };
  const int mainMenuLength = sizeof(mainMenu) / sizeof(mainMenu[0]);
  
  const char* buzzerMenu[] = { "Giorno's Theme", "Back" };
  const int buzzerMenuLength = sizeof(buzzerMenu) / sizeof(buzzerMenu[0]);
  
  const char* subMenu[] = { "Do nothing", "Back" };
  const int subMenuLength = sizeof(subMenu) / sizeof(subMenu[0]);

  const char* sniffMenu[] = { "Packet Sniffing", "Access Points", "Settings", "Back" };
  const int sniffMenuLength = 4; // Explicitly define the size of the array


}

namespace DisplayUI {
  // Initialize our global UI state variables
  MenuState menuState = MAIN_MENU;
  int mainSelection = 0;
  int subSelection = 0;
  int sniffSelection = 0;
  SniffPacketView sniffPacketView = INFO_VIEW;
  int sniffPacketSelection = 0;
  int rssiIndex = 0; // Initialize rssiIndex to track the head of the RSSI history buffer
  const int rssiHistorySize = 128; // Define the size of the RSSI history buffer
  int settingsSelection = 0;
  int modeSelection = 0;    // 0 = scan, 1 = channel
  int channelSelection = 0; // 0 = CH1, ..., 10 = CH11
  int speedSelection = 2;   // default = 3s



  void renderHeader(Adafruit_SSD1306 &display, const char* label) {
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.fillRect(0, 0, SCREEN_WIDTH, 10, SSD1306_BLACK); // Clear header area
    display.println(label);
    // Display current channel on the right side of the header
    display.setCursor(SCREEN_WIDTH - 40, 0);
    display.print("CH:");
    display.print(Sniffer::getCurrentChannel());
    display.setCursor(0, 16); // Move cursor below header
    display.drawLine(0, 11, SCREEN_WIDTH, 11, SSD1306_WHITE);
  }
  
  
  
  void begin(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
  }

  void renderSettingsList(Adafruit_SSD1306 &display, const char* const* options, int optionCount, int selectedIndex) {
    for (int i = 0; i < optionCount; i++) {
      display.setCursor(0, 16 + i * 10);
      display.print((i == selectedIndex) ? "> " : "  ");
      display.print(options[i]);
    }
  }
  
  
  void update(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
    
    switch (menuState) {
      case MAIN_MENU:
        renderHeader(display, "Main Menu");
        for (int i = 0; i < mainMenuLength; i++) {
          display.print((i == mainSelection) ? "> " : "  ");
          display.println(mainMenu[i]);
        }
        break;
    
      case INFO_MODE:
        renderHeader(display, "Info Mode");
        display.println("System Stats...");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
    
      case SILENT_MODE:
        renderHeader(display, "Silent Mode");
        display.println("Buzzer/LEDs Off");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
    
      case BUZZER_MENU:
        renderHeader(display, "Buzzer Songs");
        for (int i = 0; i < buzzerMenuLength; i++) {
          display.print((i == subSelection) ? "> " : "  ");
          display.println(buzzerMenu[i]);
        }
        break;
    
      case SUB_MENU:
        renderHeader(display, "Options");
        for (int i = 0; i < subMenuLength; i++) {
          display.print((i == subSelection) ? "> " : "  ");
          display.println(subMenu[i]);
        }
        break;
    
      case SNIFF_MENU:
        renderHeader(display, "Sniffing Menu");
        for (int i = 0; i < sniffMenuLength; i++) {
          display.print((i == sniffSelection) ? "> " : "  ");
          display.println(sniffMenu[i]);
        }
        break;
    
      case SNIFF_SETTINGS: {
        renderHeader(display, "Settings");
        const char* settingsOptions[] = { "Mode", "Channel", "Scan Speed", "Back" };
        renderSettingsList(display, settingsOptions, 4, settingsSelection);
        break;
      }
    
      case SNIFF_SETTINGS_MODE: {
        renderHeader(display, "Mode");
        const char* modeOptions[] = { "Scan", "Channel" };
        for (int i = 0; i < 2; i++) {
          display.setCursor(0, 16 + i * 10);
          display.print((i == modeSelection) ? "* " : "  ");
          display.println(modeOptions[i]);
        }
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
      }
    
      case SNIFF_SETTINGS_CHANNEL: {
        renderHeader(display, "Channel");
        for (int i = 0; i < 11; i++) {
          display.setCursor(0, 16 + i * 10);
          display.print((i == channelSelection) ? "* " : "  ");
          display.print("CH ");
          display.println(i + 1);
        }
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
      }
    
      case SNIFF_SETTINGS_SPEED: {
        renderHeader(display, "Scan Speed");
        const char* speedOptions[] = { "1s", "2s", "3s", "5s", "8s", "10s" };
        for (int i = 0; i < 6; i++) {
          display.setCursor(0, 16 + i * 10);
          display.print((i == speedSelection) ? "* " : "  ");
          display.println(speedOptions[i]);
        }
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
      }
    
      case SNIFF_PACKET: {
        renderHeader(display, "Sniffing...");
    
        if (sniffPacketView == GRAPH_VIEW) {
          const int* rssiBuf = Sniffer::getRSSIHistory();
          for (int x = 0; x < SCREEN_WIDTH; x++) {
            int i = (rssiIndex + x) % rssiHistorySize;
            int rssi = rssiBuf[i];
            int y = map(rssi, -100, -30, SCREEN_HEIGHT - 1, 16);
            y = constrain(y, 16, SCREEN_HEIGHT - 1);
            display.drawPixel(x, y, SSD1306_WHITE);
          }
          display.setCursor(0, SCREEN_HEIGHT - 10);
          display.print("> Back");
          break;
        }
    
        const int totalItems = 5;
        if (sniffPacketSelection < 0) sniffPacketSelection = 0;
        if (sniffPacketSelection >= totalItems) sniffPacketSelection = totalItems - 1;
    
        for (int i = 0; i < totalItems; i++) {
          display.setCursor(0, 16 + i * 10);
          display.print((i == sniffPacketSelection) ? "> " : "  ");
          switch (i) {
            case 0:
              display.print("Packets: ");
              display.print(Sniffer::getPacketCount());
              break;
            case 1:
              display.print("Last MAC: ");
              display.print(Sniffer::getLastMAC());
              break;
            case 2:
              display.print("RSSI: ");
              display.print(Sniffer::getLastRSSI());
              break;
            case 3:
              display.print("Graph View");
              break;
            case 4:
              display.print("Back");
              break;
          }
        }
        break;
      }
    
      case SNIFF_AP:
        renderHeader(display, "Access Points");
        display.println("Not yet implemented");
        display.setCursor(0, SCREEN_HEIGHT - 10);
        display.print("> Back");
        break;
    
      case FILE_BROWSER:
        renderHeader(display, "File Browser");
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
