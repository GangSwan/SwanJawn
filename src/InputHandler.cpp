#include "InputHandler.h"
#include "Sniffer.h"
#include "FileBrowser.h"

// Create a rotary encoder instance locally
RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT, RotaryEncoder::LatchMode::TWO03);
static int lastPos = 0;
static bool wasPressed = false;

namespace InputHandler {

  void begin() {
    encoder.setPosition(0);
    pinMode(ENCODER_BTN, INPUT_PULLUP);
  }

  // Helper function to handle rotary encoder changes
  void handleEncoderChange(int& selection, int itemCount, int newPos) {
    if (newPos != lastPos) {
      selection = abs(newPos) % itemCount;
      lastPos = newPos;
    }
  }

  // Handle rotary encoder changes for specific menu states
  void processRotaryEncoder(int newPos) {
    switch (DisplayUI::menuState) {
      case DisplayUI::MAIN_MENU:
        handleEncoderChange(DisplayUI::mainSelection, 5, newPos);
        break;
      case DisplayUI::SUB_MENU:
      case DisplayUI::BUZZER_MENU:
        handleEncoderChange(DisplayUI::subSelection, 2, newPos);
        break;
      case DisplayUI::SNIFF_MENU:
        handleEncoderChange(DisplayUI::sniffSelection, 4, newPos);
        break;
      case DisplayUI::FILE_BROWSER:
        if (FileBrowser::browserState == FileBrowser::FILE_LIST) {
          if (newPos > lastPos) FileBrowser::nextFile();
          else if (newPos < lastPos) FileBrowser::previousFile();
          lastPos = newPos;
        } else if (FileBrowser::browserState == FileBrowser::FILE_VIEW) {
          if (newPos > lastPos) FileBrowser::nextPage();
          else if (newPos < lastPos) FileBrowser::previousPage();
          lastPos = newPos;
        }
        break;
      case DisplayUI::SNIFF_SETTINGS:
        handleEncoderChange(DisplayUI::settingsSelection, 4, newPos);
        break;
      case DisplayUI::SNIFF_SETTINGS_MODE:
        handleEncoderChange(DisplayUI::modeSelection, 2, newPos);
        break;
      case DisplayUI::SNIFF_SETTINGS_CHANNEL:
        handleEncoderChange(DisplayUI::channelSelection, 11, newPos);
        break;
      case DisplayUI::SNIFF_SETTINGS_SPEED:
        handleEncoderChange(DisplayUI::speedSelection, 6, newPos);
        break;
      default:
        break;
    }
  }

  // Handle serial input for menu navigation
  void processSerialInput(char input) {
    switch (DisplayUI::menuState) {
      case DisplayUI::MAIN_MENU:
        if (input == 'w') DisplayUI::mainSelection = (DisplayUI::mainSelection - 1 + 5) % 5;
        else if (input == 's') DisplayUI::mainSelection = (DisplayUI::mainSelection + 1) % 5;
        else if (input == 'e') {
          switch (DisplayUI::mainSelection) {
            case 0:
              DisplayUI::menuState = DisplayUI::SNIFF_MENU;
              DisplayUI::sniffSelection = 0;
              break;
            case 3:
              DisplayUI::menuState = DisplayUI::BUZZER_MENU;
              DisplayUI::subSelection = 0;
              break;
            case 4:
              DisplayUI::menuState = DisplayUI::FILE_BROWSER;
              FileBrowser::initFileBrowser();
              break;
            default:
              DisplayUI::menuState = DisplayUI::SUB_MENU;
              DisplayUI::subSelection = 0;
              break;
          }
          encoder.setPosition(0);
        }
        break;
      case DisplayUI::SUB_MENU:
      case DisplayUI::BUZZER_MENU:
        if (input == 'w') DisplayUI::subSelection = (DisplayUI::subSelection - 1 + 2) % 2;
        else if (input == 's') DisplayUI::subSelection = (DisplayUI::subSelection + 1) % 2;
        else if (input == 'e') {
          if (DisplayUI::subSelection == 1) {
            DisplayUI::menuState = DisplayUI::MAIN_MENU;
            DisplayUI::mainSelection = 0;
          } else if (DisplayUI::menuState == DisplayUI::BUZZER_MENU) {
            SoundFX::playGiornosTheme();
          }
          encoder.setPosition(0);
        }
        break;
      case DisplayUI::SNIFF_MENU:
        if (input == 'w') DisplayUI::sniffSelection = (DisplayUI::sniffSelection - 1 + 4) % 4;
        else if (input == 's') DisplayUI::sniffSelection = (DisplayUI::sniffSelection + 1) % 4;
        else if (input == 'e') {
          encoder.setPosition(0);
          switch (DisplayUI::sniffSelection) {
            case 0:
              DisplayUI::menuState = DisplayUI::SNIFF_PACKET;
              break;
            case 1:
              DisplayUI::menuState = DisplayUI::SNIFF_AP;
              break;
            case 2:
              DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS;
              DisplayUI::settingsSelection = 0;
              break;
            case 3:
              DisplayUI::menuState = DisplayUI::MAIN_MENU;
              DisplayUI::mainSelection = 0;
              break;
          }
        }
        break;
      case DisplayUI::SNIFF_PACKET:
        if (input == 'w') DisplayUI::sniffPacketSelection = max(DisplayUI::sniffPacketSelection - 1, 0);
        else if (input == 's') DisplayUI::sniffPacketSelection = min(DisplayUI::sniffPacketSelection + 1, 4);
        else if (input == 'e') {
          if (DisplayUI::sniffPacketSelection == 3) {
            DisplayUI::sniffPacketView = (DisplayUI::sniffPacketView == DisplayUI::INFO_VIEW)
                                         ? DisplayUI::GRAPH_VIEW
                                         : DisplayUI::INFO_VIEW;
          } else if (DisplayUI::sniffPacketSelection == 4) {
            DisplayUI::menuState = DisplayUI::SNIFF_MENU;
            DisplayUI::sniffSelection = 0;
          }
        }
        break;
      case DisplayUI::SNIFF_SETTINGS:
        if (input == 'w') DisplayUI::settingsSelection = (DisplayUI::settingsSelection - 1 + 4) % 4;
        else if (input == 's') DisplayUI::settingsSelection = (DisplayUI::settingsSelection + 1) % 4;
        else if (input == 'e') {
          switch (DisplayUI::settingsSelection) {
            case 0: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_MODE; break;
            case 1: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_CHANNEL; break;
            case 2: DisplayUI::menuState = DisplayUI::SNIFF_SETTINGS_SPEED; break;
            case 3: DisplayUI::menuState = DisplayUI::SNIFF_MENU; break;
          }
        }
        break;
      default:
        break;
    }
  }

  // Handle physical button press
  void processButtonPress() {
    bool isPressed = digitalRead(ENCODER_BTN) == LOW;
    if (isPressed && !wasPressed) {
      wasPressed = true;
      processSerialInput('e'); // Mimic 'e' key press
    } else if (!isPressed) {
      wasPressed = false;
    }
  }

  void update() {
    // Process rotary encoder ticks
    for (int i = 0; i < 3; i++) {
      encoder.tick();
    }
    int newPos = encoder.getPosition();

    // Handle rotary encoder changes
    processRotaryEncoder(newPos);

    // Handle serial input
    if (Serial.available()) {
      char input = Serial.read();
      processSerialInput(input);
    }

    // Handle physical button press
    processButtonPress();
  }
}
