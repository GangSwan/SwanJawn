#include "InputHandler.h"

// Create a rotary encoder instance locally
RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT, RotaryEncoder::LatchMode::TWO03);
static int lastPos = 0;
static bool wasPressed = false;

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
    
    // Update selection indices based on the current menu state:
    if (DisplayUI::menuState == MAIN_MENU) {
      if (newPos != lastPos) {
        DisplayUI::mainSelection = abs(newPos) % 4; // 4 items in mainMenu
        lastPos = newPos;
      }
    }
    else if (DisplayUI::menuState == SUB_MENU || DisplayUI::menuState == BUZZER_MENU) {
      if (newPos != lastPos) {
        // Both SUB_MENU and BUZZER_MENU have 2 items
        DisplayUI::subSelection = abs(newPos) % 2;
        lastPos = newPos;
      }
    }
    else if (DisplayUI::menuState == SNIFF_MENU) {
      if (newPos != lastPos) {
        // SNIFF_MENU has 3 items: Packet Sniffing, Access Points, Back
        DisplayUI::sniffSelection = abs(newPos) % 3;
        lastPos = newPos;
      }
    }
    
    // Process serial inputs for navigation:
    if (Serial.available()) {
      char input = Serial.read();
      
      // MAIN MENU input
      if (DisplayUI::menuState == MAIN_MENU) {
        if (input == 'w')
          DisplayUI::mainSelection = (DisplayUI::mainSelection - 1 + 4) % 4;
        if (input == 's')
          DisplayUI::mainSelection = (DisplayUI::mainSelection + 1) % 4;
        if (input == 'e') {
          if (DisplayUI::mainSelection == 0) { // Sniffing
            DisplayUI::menuState = SNIFF_MENU;
            DisplayUI::sniffSelection = 0;
            encoder.setPosition(0);
            Serial.println("Entering Sniffing menu...");
          }
          else if (DisplayUI::mainSelection == 3) { // Buzzer Songs
            DisplayUI::menuState = BUZZER_MENU;
            DisplayUI::subSelection = 0;
            encoder.setPosition(0);
            Serial.println("Entering Buzzer Songs menu...");
          }
          else { // Info Mode or Silent Mode
            DisplayUI::menuState = SUB_MENU;
            DisplayUI::subSelection = 0;
            encoder.setPosition(0);
            Serial.println("Entering submenu...");
          }
        }
      }
      // BUZZER_MENU or SUB_MENU input
      else if (DisplayUI::menuState == BUZZER_MENU || DisplayUI::menuState == SUB_MENU) {
        if (input == 'w')
          DisplayUI::subSelection = (DisplayUI::subSelection - 1 + 2) % 2;
        if (input == 's')
          DisplayUI::subSelection = (DisplayUI::subSelection + 1) % 2;
        if (input == 'e') {
          // "Back" is index 1
          if (DisplayUI::subSelection == 1) {
            DisplayUI::menuState = MAIN_MENU;
            encoder.setPosition(DisplayUI::mainSelection);
            Serial.println("Returning to Main Menu...");
          }
          else {
            // In BUZZER_MENU, index 0 plays Giorno's Theme; in SUB_MENU, nothing is done (placeholder)
            if (DisplayUI::menuState == BUZZER_MENU && DisplayUI::subSelection == 0) {
              Serial.println("Playing Giorno’s Theme...");
              SoundFX::playGiornosTheme();
            } else {
              Serial.println("Selected 'Do nothing'");
            }
          }
        }
      }
      // SNIFF_MENU input
      else if (DisplayUI::menuState == SNIFF_MENU) {
        if (input == 'w')
          DisplayUI::sniffSelection = (DisplayUI::sniffSelection - 1 + 3) % 3;
        if (input == 's')
          DisplayUI::sniffSelection = (DisplayUI::sniffSelection + 1) % 3;
        if (input == 'e') {
          if (DisplayUI::sniffSelection == 0) {  // Packet Sniffing
            DisplayUI::menuState = SNIFF_PACKET;
            Serial.println("Entering Packet Sniffing mode...");
          }
          else if (DisplayUI::sniffSelection == 1) {  // Access Points
            DisplayUI::menuState = SNIFF_AP;
            Serial.println("Entering Access Points mode...");
          }
          else if (DisplayUI::sniffSelection == 2) {  // Back
            DisplayUI::menuState = MAIN_MENU;
            Serial.println("Returning to Main Menu...");
          }
        }
      }
      // In SNIFF_PACKET or SNIFF_AP, only a "Back" option is available:
      else if (DisplayUI::menuState == SNIFF_PACKET || DisplayUI::menuState == SNIFF_AP) {
        if (input == 'e') {
          DisplayUI::menuState = SNIFF_MENU;
          DisplayUI::sniffSelection = 0;
          encoder.setPosition(0);
          Serial.println("Exiting to Sniffing menu...");
        }
      }
    }
    
    // Handle physical button presses (mimicking the 'e' key)
    bool isPressed = digitalRead(ENCODER_BTN) == LOW;
    if (isPressed && !wasPressed) {
      wasPressed = true;
      if (DisplayUI::menuState == MAIN_MENU) {
         if (DisplayUI::mainSelection == 0) {
           DisplayUI::menuState = SNIFF_MENU;
           DisplayUI::sniffSelection = 0;
           encoder.setPosition(0);
           Serial.println("Entering Sniffing menu...");
         }
         else if (DisplayUI::mainSelection == 3) {
           DisplayUI::menuState = BUZZER_MENU;
           DisplayUI::subSelection = 0;
           encoder.setPosition(0);
           Serial.println("Entering Buzzer Songs menu...");
         }
         else {
           DisplayUI::menuState = SUB_MENU;
           DisplayUI::subSelection = 0;
           encoder.setPosition(0);
           Serial.println("Entering submenu...");
         }
      }
      else if (DisplayUI::menuState == BUZZER_MENU || DisplayUI::menuState == SUB_MENU) {
         if (DisplayUI::subSelection == 1) {
             DisplayUI::menuState = MAIN_MENU;
             encoder.setPosition(DisplayUI::mainSelection);
             Serial.println("Returning to Main Menu...");
         }
         else {
             if (DisplayUI::menuState == BUZZER_MENU && DisplayUI::subSelection == 0) {
                 Serial.println("Playing Giorno’s Theme...");
                 SoundFX::playGiornosTheme();
             }
         }
      }
      else if (DisplayUI::menuState == SNIFF_MENU) {
         if (DisplayUI::sniffSelection == 0) {
             DisplayUI::menuState = SNIFF_PACKET;
             Serial.println("Entering Packet Sniffing mode...");
         }
         else if (DisplayUI::sniffSelection == 1) {
             DisplayUI::menuState = SNIFF_AP;
             Serial.println("Entering Access Points mode...");
         }
         else if (DisplayUI::sniffSelection == 2) {
             DisplayUI::menuState = MAIN_MENU;
             Serial.println("Returning to Main Menu...");
         }
      }
      else if (DisplayUI::menuState == SNIFF_PACKET || DisplayUI::menuState == SNIFF_AP) {
         DisplayUI::menuState = SNIFF_MENU;
         DisplayUI::sniffSelection = 0;
         encoder.setPosition(0);
         Serial.println("Exiting to Sniffing menu...");
      }
    }
    else if (!isPressed) {
      wasPressed = false;
    }
  }
}
