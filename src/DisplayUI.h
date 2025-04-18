#ifndef DISPLAYUI_H
#define DISPLAYUI_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"

namespace DisplayUI {
  // Define the menu states with our updated structure:
  enum MenuState { 
    MAIN_MENU,      // Main menu: Sniffing, Info Mode, Silent Mode, Buzzer Songs
    INFO_MODE,      // Dedicated Info Mode screen
    SILENT_MODE,    // Dedicated Silent Mode screen
    SUB_MENU,       // Submenu for additional options (for Info and Silent modes, if needed)
    BUZZER_MENU,    // Buzzer Songs menu (includes Giorno’s Theme and Back)
    SNIFF_MENU,     // Sniffing submenu (displays "Packet Sniffing," "Access Points," "Back")
    SNIFF_PACKET,   // Packet Sniffing display (a simple graph of fake packets)
    SNIFF_AP,       // Access Points display (placeholder for future expansion)
    FILE_BROWSER,
    SNIFF_SETTINGS,
    SNIFF_SETTINGS_MODE,
    SNIFF_SETTINGS_CHANNEL,
    SNIFF_SETTINGS_SPEED,

        // File browser for SD card (if needed)
  };

  enum SniffPacketView {
    INFO_VIEW,
    GRAPH_VIEW
  };
  
  extern SniffPacketView sniffPacketView;
  extern int sniffPacketSelection;

  

  // Global UI state variables (accessible across modules)
  extern MenuState menuState;
  extern int mainSelection;      // For MAIN_MENU (4 items)
  extern int subSelection;       // For SUB_MENU and BUZZER_MENU (typically 2 items)
  extern int sniffSelection; 
  extern int settingsSelection;
  extern int modeSelection;
  extern int channelSelection;
  extern int speedSelection;
    
  
  void begin(Adafruit_SSD1306 &display);
  void update(Adafruit_SSD1306 &display);
}

#endif // DISPLAYUI_H
