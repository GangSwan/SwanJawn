#include "BootSequence.h"
#include "Storage.h"
#include "SplashBitMap.h"

void BootSequence::showSplash(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.drawBitmap(0, 0, swanBitmap, 128, 64, SSD1306_WHITE);
  display.display();
  delay(1000);  // Show splash for 1 second
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


