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
    encoder.tick();
    int newPos = encoder.getPosition();

    // Update cursor position based on encoder movement
    if (newPos > lastPos) {
        DisplayUI::cursorPosition++;
    } else if (newPos < lastPos) {
        DisplayUI::cursorPosition--;
    }
    lastPos = newPos;

    // Clamp cursorPosition to valid bounds based on the current menu
    switch (DisplayUI::menuState) {
        case DisplayUI::MAIN_MENU:
            DisplayUI::cursorPosition = (DisplayUI::cursorPosition + 5) % 5;  // Wrap around 5 items
            break;
        case DisplayUI::SNIFF_MENU:
            DisplayUI::cursorPosition = (DisplayUI::cursorPosition + 3) % 3;  // Wrap around 3 items
            break;
        case DisplayUI::SNIFF_PACKET:
            DisplayUI::cursorPosition = constrain(DisplayUI::cursorPosition, 0, 4);  // Limit to 5 items
            break;
        // Add cases for other menus as needed
        default:
            break;
    }

    // Handle button press
    bool isPressed = digitalRead(ENCODER_BTN) == LOW;
    if (isPressed && !wasPressed) {
        wasPressed = true;

        switch (DisplayUI::menuState) {
            case DisplayUI::MAIN_MENU:
                switch (DisplayUI::cursorPosition) {
                    case 0:
                        DisplayUI::menuState = DisplayUI::SNIFF_MENU;
                        DisplayUI::cursorPosition = 0;
                        Serial.println("Entering Sniffing menu...");
                        break;
                    case 3:
                        DisplayUI::menuState = DisplayUI::BUZZER_MENU;
                        DisplayUI::cursorPosition = 0;
                        Serial.println("Entering Buzzer Songs menu...");
                        break;
                    case 4:
                        DisplayUI::menuState = DisplayUI::FILE_BROWSER;
                        FileBrowser::initFileBrowser();
                        DisplayUI::cursorPosition = 0;
                        Serial.println("Entering File Browser...");
                        break;
                    default:
                        DisplayUI::menuState = DisplayUI::SUB_MENU;
                        DisplayUI::cursorPosition = 0;
                        Serial.println("Entering submenu...");
                        break;
                }
                break;

            case DisplayUI::SNIFF_PACKET:
                if (DisplayUI::cursorPosition == 3) {  // "Graph View"
                    DisplayUI::sniffPacketView = (DisplayUI::sniffPacketView == DisplayUI::INFO_VIEW)
                                                 ? DisplayUI::GRAPH_VIEW
                                                 : DisplayUI::INFO_VIEW;
                    Serial.println("Toggled to " + String(DisplayUI::sniffPacketView == DisplayUI::INFO_VIEW ? "Info" : "Graph") + " View");
                } else if (DisplayUI::cursorPosition == 4) {  // "Back"
                    DisplayUI::menuState = DisplayUI::SNIFF_MENU;
                    DisplayUI::cursorPosition = 0;
                    Serial.println("Returning to Sniff Menu...");
                }
                break;

            // Add cases for other menus as needed
            default:
                break;
        }
    } else if (!isPressed) {
        wasPressed = false;
    }
  }
} // End of namespace InputHandler
