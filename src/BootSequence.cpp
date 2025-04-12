#include "BootSequence.h"

void BootSequence::showSplash(Adafruit_SSD1306 &display) {
  const char* logo[] = {
    "   _____ _____ ",
    "  / ____|_   _|",
    " | (___   | |  ",
    "  \\___ \\  | |  ",
    "  ____) |_| |_ ",
    " |_____/|_____|",
    "     S J v0.1  "
  };

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  for (int i = 0; i < 7; i++) {
    display.setCursor(0, i * 8);
    display.println(logo[i]);
    display.display();
    delay(150);
  }
  delay(1000);
}

void BootSequence::playStartupTone() {
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 659, 120); delay(15);
  tone(BUZZER_PIN, 784, 120); delay(15);
  tone(BUZZER_PIN, 880, 150); delay(15);
}

void BootSequence::begin(Adafruit_SSD1306 &display) {
  showSplash(display);
  playStartupTone();
}
