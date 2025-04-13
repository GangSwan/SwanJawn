#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Config.h"
#include "BootSequence.h"
#include "DisplayUI.h"
#include "InputHandler.h"
#include "SoundFX.h"
#include "Storage.h"

// Create the global OLED display instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Simulated battery variables (for RGB LED simulation)
static int battery = 0;
static int direction = 1;
static unsigned long lastUpdate = 0;





void setup() {
  Serial.begin(115200);
  
  if (Storage::begin()) {
    Serial.println("SD card initialized successfully.");
    Storage::logBoot("SWAN-OS v0.1 Boot OK");
  } else {
    Serial.println("SD card initialization failed.");
    // TODO: Display error message on OLED?
  }
  
  // Continue with BootSequence, UI, etc...
  
  // Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Boot sequence: show splash screen and play startup tone
  BootSequence::begin(display);
  
  // Initialize UI and input modules
  DisplayUI::begin(display);
  InputHandler::begin();
}

void loop() {
  // Update input handling (rotary encoder, serial, button)
  InputHandler::update();
  
  // Simulate battery level and update RGB LED status (for demonstration)
  if (millis() - lastUpdate > 100) {
    lastUpdate = millis();
    battery += direction;
    if (battery >= 100) direction = -1;
    if (battery <= 0) direction = 1;
    
    if (battery > 65) {
      analogWrite(RED_LED, 0);
      analogWrite(GREEN_LED, 255);
    } else if (battery > 30) {
      analogWrite(RED_LED, 255);
      analogWrite(GREEN_LED, 255);
    } else {
      analogWrite(RED_LED, 255);
      analogWrite(GREEN_LED, 0);
    }
  }
  
  // Update the OLED display with the current UI state
  DisplayUI::update(display);
}
