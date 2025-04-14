#include "InputHandler.h"
#include "Sniffer.h"
#include "FileBrowser.h"  // Include the file browser header


// Create a rotary encoder instance locally
RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT, RotaryEncoder::LatchMode::TWO03);
static int lastPos = 0;
static bool wasPressed = false;
static bool graphMode = false;

namespace InputHandler {

  void begin() {
    encoder.setPosition(0);
    pinMode(ENCODER_BTN, INPUT_PULLUP);
  }
  
  void update() {
    // Process rotary encoder ticks
    for (int i = 0; i < 3; i++) {
      encoder.tick();
    }
    int newPos = encoder.getPosition();
    
    // --- Process rotary encoder changes ---

    if (DisplayUI::menuState == DisplayUI::SNIFF_PACKET) {
      if (newPos > lastPos) {
        DisplayUI::sniffPacketSelection = min(DisplayUI::sniffPacketSelection + 1, 4);
      } else if (newPos < lastPos) {
        DisplayUI::sniffPacketSelection = max(DisplayUI::sniffPacketSelection - 1, 0);
      }
      lastPos = newPos;
    }
    
    
    

    switch (DisplayUI::menuState) {
      case DisplayUI::MAIN_MENU:
        if (newPos != lastPos) {
          // MAIN_MENU now has 5 items (the last one being "File Browser")
          DisplayUI::mainSelection = abs(newPos) % 5;
          lastPos = newPos;
        }
        break;
      case DisplayUI::SUB_MENU:
      case DisplayUI::BUZZER_MENU:
        if (newPos != lastPos) {
          // Both SUB_MENU and BUZZER_MENU have 2 items
          DisplayUI::subSelection = abs(newPos) % 2;
          lastPos = newPos;
        }
        break;
      case DisplayUI::SNIFF_MENU:
        if (newPos != lastPos) {
          // SNIFF_MENU has 3 items: Packet Sniffing, Access Points, Back
          DisplayUI::sniffSelection = abs(newPos) % 4;
          lastPos = newPos;
        }
        break;
      case DisplayUI::FILE_BROWSER:
        switch (FileBrowser::browserState) {
          case FileBrowser::FILE_LIST:
            if (newPos != lastPos) {
              if (newPos > lastPos)
                FileBrowser::nextFile();
              else
                FileBrowser::previousFile();
              lastPos = newPos;
            }
            break;
          case FileBrowser::FILE_VIEW:
            if (newPos != lastPos) {
              if (newPos > lastPos)
                FileBrowser::nextPage();
              else
                FileBrowser::previousPage();
              lastPos = newPos;
            }
            break;

            case DisplayUI::SNIFF_SETTINGS:
            if (newPos != lastPos) {
              DisplayUI::settingsSelection = abs(newPos) % 4;
              lastPos = newPos;
            }
            break;

          case DisplayUI::SNIFF_SETTINGS_MODE:
            if (newPos != lastPos) {
              DisplayUI::modeSelection = abs(newPos) % 2;
              lastPos = newPos;
            }
            break;

          case DisplayUI::SNIFF_SETTINGS_CHANNEL:
            if (newPos != lastPos) {
              DisplayUI::channelSelection = abs(newPos) % 11;
              lastPos = newPos;
            }
            break;

          case DisplayUI::SNIFF_SETTINGS_SPEED:
            if (newPos != lastPos) {
              DisplayUI::speedSelection = abs(newPos) % 6;
              lastPos = newPos;
            }
            break;

          default:
            break;
        }
        break;
      default:
        break;
    } // End rotary encoder switch

    
    // --- Process Serial Inputs ---
if (Serial.available()) {
  char input = Serial.read();
  switch (DisplayUI::menuState) {

    case DisplayUI::MAIN_MENU:
      switch (input) {
        case 'w':
          DisplayUI::mainSelection = (DisplayUI::mainSelection - 1 + 5) % 5;
          break;
        case 's':
          DisplayUI::mainSelection = (DisplayUI::mainSelection + 1) % 5;
          break;
        case 'e':
          switch (DisplayUI::mainSelection) {
            case 0: // Sniffing
              DisplayUI::menuState = DisplayUI::SNIFF_MENU;
              DisplayUI::sniffSelection = 0;
              encoder.setPosition(0);
              Serial.println("Entering Sniffing menu...");
              break;
            case 3: // Buzzer Songs
              DisplayUI::menuState = DisplayUI::BUZZER_MENU;
              DisplayUI::subSelection = 0;
              encoder.setPosition(0);
              Serial.println("Entering Buzzer Songs menu...");
              break;
            case 4: // File Browser
              DisplayUI::menuState = DisplayUI::FILE_BROWSER;
              FileBrowser::initFileBrowser();
              encoder.setPosition(0);
              Serial.println("Entering File Browser...");
              break;
            default:
              DisplayUI::menuState = DisplayUI::SUB_MENU;
              DisplayUI::subSelection = 0;
              encoder.setPosition(0);
              Serial.println("Entering submenu...");
              break;
          }
          break;
      }
      break;

    case DisplayUI::BUZZER_MENU:
    case DisplayUI::SUB_MENU:
      switch (input) {
        case 'w':
          DisplayUI::subSelection = (DisplayUI::subSelection - 1 + 2) % 2;
          break;
        case 's':
          DisplayUI::subSelection = (DisplayUI::subSelection + 1) % 2;
          break;
        case 'e':
          if (DisplayUI::subSelection == 1) {
            DisplayUI::menuState = DisplayUI::MAIN_MENU;
            DisplayUI::mainSelection = 0;
            encoder.setPosition(0);
            Serial.println("Returning to Main Menu...");
          } else if (DisplayUI::menuState == DisplayUI::BUZZER_MENU) {
            Serial.println("Playing Giorno’s Theme...");
            SoundFX::playGiornosTheme();
          }
          break;
      }
      break;

    case DisplayUI::SNIFF_MENU:
      switch (input) {
        case 'w':
          DisplayUI::sniffSelection = (DisplayUI::sniffSelection - 1 + 4) % 4;
          break;
        case 's':
          DisplayUI::sniffSelection = (DisplayUI::sniffSelection + 1) % 4;
          break;
        case 'e':
          encoder.setPosition(0);
          DisplayUI::sniffPacketSelection = 0;
          switch (DisplayUI::sniffSelection) {
            case 0:
              DisplayUI::menuState = DisplayUI::SNIFF_PACKET;
              Serial.println("Entering Packet Sniffing mode...");
              break;
            case 1:
              DisplayUI::menuState = DisplayUI::SNIFF_AP;
              Serial.println("Entering Access Points mode...");
              break;
            case 2:
              DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
              DisplayUI::settingsSelection = 0;
              Serial.println("Entering Settings menu...");
              break;
            case 3:
              DisplayUI::menuState = DisplayUI::MAIN_MENU;
              DisplayUI::mainSelection = 0;
              Serial.println("Returning to Main Menu...");
              break;
          }
          break;
      }
      break;

    case DisplayUI::SNIFF_PACKET:
      if (input == 'w')
        DisplayUI::sniffPacketSelection = max(DisplayUI::sniffPacketSelection - 1, 0);
      else if (input == 's')
        DisplayUI::sniffPacketSelection = min(DisplayUI::sniffPacketSelection + 1, 4);
      else if (input == 'e') {
        if (DisplayUI::sniffPacketSelection == 3) {
          DisplayUI::sniffPacketView = (DisplayUI::sniffPacketView == DisplayUI::INFO_VIEW)
                                       ? DisplayUI::GRAPH_VIEW
                                       : DisplayUI::INFO_VIEW;
          Serial.println("Switched to " + String(DisplayUI::sniffPacketView == DisplayUI::INFO_VIEW ? "Info" : "Graph") + " View");
        } else if (DisplayUI::sniffPacketSelection == 4) {
          DisplayUI::menuState = DisplayUI::SNIFF_MENU;
          DisplayUI::sniffSelection = 0;
          encoder.setPosition(0);
          Serial.println("Returning to Sniff Menu");
        }
      }
      break;

    case DisplayUI::SNIFF_SETTINGS:
      if (input == 'w')
        DisplayUI::settingsSelection = (DisplayUI::settingsSelection - 1 + 4) % 4;
      else if (input == 's')
        DisplayUI::settingsSelection = (DisplayUI::settingsSelection + 1) % 4;
      else if (input == 'e') {
        switch (DisplayUI::settingsSelection) {
          case 0: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_MODE; break;
          case 1: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_CHANNEL; break;
          case 2: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_SPEED; break;
          case 3: DisplayUI::menuState = DisplayUI::SNIFF_MENU; break;
        }
      }
      break;

    case DisplayUI::SNIFF_SETTINGS_MODE:
      if (input == 'w' || input == 's')
        DisplayUI::modeSelection = (DisplayUI::modeSelection + 1) % 2;
      else if (input == 'e')
        DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
      break;

    case DisplayUI::SNIFF_SETTINGS_CHANNEL:
      if (input == 'w')
        DisplayUI::channelSelection = (DisplayUI::channelSelection - 1 + 11) % 11;
      else if (input == 's')
        DisplayUI::channelSelection = (DisplayUI::channelSelection + 1) % 11;
      else if (input == 'e')
        DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
      break;

    case DisplayUI::SNIFF_SETTINGS_SPEED:
      if (input == 'w')
        DisplayUI::speedSelection = (DisplayUI::speedSelection - 1 + 6) % 6;
      else if (input == 's')
        DisplayUI::speedSelection = (DisplayUI::speedSelection + 1) % 6;
      else if (input == 'e')
        DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
      break;

    case DisplayUI::FILE_BROWSER:
      if (input == 'e') {
        if (FileBrowser::selectedFileIndex == FileBrowser::fileCount) {
          DisplayUI::menuState = DisplayUI::MAIN_MENU;
          DisplayUI::mainSelection = 0;
          encoder.setPosition(0);
          Serial.println("Exiting File Browser...");
        } else {
          FileBrowser::openSelectedFile();
          Serial.println("Opening file...");
        }
      }
      break;

    default:
      break;
  }
} // End serial input processing


// --- Process Physical Button (mimicking the 'e' key) ---
bool isPressed = digitalRead(ENCODER_BTN) == LOW;
if (isPressed && !wasPressed) {
  wasPressed = true;
  switch (DisplayUI::menuState) {
    case DisplayUI::MAIN_MENU:
      switch (DisplayUI::mainSelection) {
        case 0:
          DisplayUI::menuState = DisplayUI::SNIFF_MENU;
          DisplayUI::sniffSelection = 0;
          encoder.setPosition(0);
          Serial.println("Entering Sniffing menu...");
          break;
        case 3:
          DisplayUI::menuState = DisplayUI::BUZZER_MENU;
          DisplayUI::subSelection = 0;
          encoder.setPosition(0);
          Serial.println("Entering Buzzer Songs menu...");
          break;
        case 4:
          DisplayUI::menuState = DisplayUI::FILE_BROWSER;
          FileBrowser::initFileBrowser();
          encoder.setPosition(0);
          Serial.println("Entering File Browser...");
          break;
        default:
          DisplayUI::menuState = DisplayUI::SUB_MENU;
          DisplayUI::subSelection = 0;
          encoder.setPosition(0);
          Serial.println("Entering submenu...");
          break;
      }
      break;
      
    case DisplayUI::BUZZER_MENU:
    case DisplayUI::SUB_MENU:
      if (DisplayUI::subSelection == 1) {
        DisplayUI::menuState = DisplayUI::MAIN_MENU;
        DisplayUI::mainSelection = 0;
        encoder.setPosition(0);
        Serial.println("Returning to Main Menu...");
      } else {
        if (DisplayUI::menuState == DisplayUI::BUZZER_MENU && DisplayUI::subSelection == 0) {
          Serial.println("Playing Giorno’s Theme...");
          SoundFX::playGiornosTheme();
        }
      }
      break;
      
      case DisplayUI::SNIFF_MENU:
      switch (DisplayUI::sniffSelection) {
        case 0:
          DisplayUI::menuState = DisplayUI::SNIFF_PACKET;
          Serial.println("Entering Packet Sniffing mode...");
          break;
        case 1:
          DisplayUI::menuState = DisplayUI::SNIFF_AP;
          Serial.println("Entering Access Points mode...");
          break;
        case 2:
          DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
          DisplayUI::settingsSelection = 0;
          Serial.println("Entering Settings menu...");
          break;
        case 3:
          DisplayUI::menuState = DisplayUI::MAIN_MENU;
          DisplayUI::mainSelection = 0;
          Serial.println("Returning to Main Menu...");
          break;
      }
      encoder.setPosition(0);
      break;
    
      
      case DisplayUI::SNIFF_PACKET:
      if (DisplayUI::sniffPacketView == DisplayUI::GRAPH_VIEW) {
        // In graph view, always return to info list
        DisplayUI::sniffPacketView = DisplayUI::INFO_VIEW;
        encoder.setPosition(DisplayUI::sniffPacketSelection);
        Serial.println("[SNIFFER] Returning to list view");
      } else {
        // In list view: act on current selection
        if (DisplayUI::sniffPacketSelection == 3) {
          DisplayUI::sniffPacketView = DisplayUI::GRAPH_VIEW;
          encoder.setPosition(0);
          Serial.println("[SNIFFER] Switched to GRAPH_VIEW");
        } else if (DisplayUI::sniffPacketSelection == 4) {
          DisplayUI::menuState = DisplayUI::SNIFF_MENU;
          DisplayUI::sniffSelection = 0;
          encoder.setPosition(0);
          Serial.println("[SNIFFER] Returning to Sniff Menu...");
        }
      }
      break;

      case DisplayUI::SNIFF_SETTINGS:
  switch (DisplayUI::settingsSelection) {
    case 0:
      DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_MODE;
      break;
    case 1:
      DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_CHANNEL;
      break;
    case 2:
      DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_SPEED;
      break;
    case 3:
      DisplayUI::menuState = DisplayUI::SNIFF_MENU;
      break;
  }
  encoder.setPosition(0);
  break;

  case DisplayUI::SNIFF_SETTINGS_MODE:
  DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
  encoder.setPosition(DisplayUI::settingsSelection);
  Serial.println("Back to Settings");
  break;

case DisplayUI::SNIFF_SETTINGS_CHANNEL:
  DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
  encoder.setPosition(DisplayUI::settingsSelection);
  Serial.println("Back to Settings");
  break;

case DisplayUI::SNIFF_SETTINGS_SPEED:
  DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
  encoder.setPosition(DisplayUI::settingsSelection);
  Serial.println("Back to Settings");
  break;


    
      
    case DisplayUI::FILE_BROWSER:
      switch (FileBrowser::browserState) {
        case FileBrowser::FILE_LIST:
          FileBrowser::openSelectedFile();
          Serial.println("Opening file...");
          break;
        case FileBrowser::FILE_VIEW:
          FileBrowser::closeFileView();
          // Also exit file browser (optional) so MAIN_MENU is shown:
          DisplayUI::menuState = DisplayUI::MAIN_MENU;
          DisplayUI::mainSelection = 0;
          Serial.println("Exiting File View to Main Menu...");
          break;
        default:
          break;
      }
      break;
      
    default:
      break;
      }
    } else if (!isPressed) {
      wasPressed = false;
    }

  } // End of update()
} // End of namespace InputHandler