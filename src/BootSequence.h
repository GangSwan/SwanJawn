#ifndef BOOTSEQUENCE_H
#define BOOTSEQUENCE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"

namespace BootSequence {
  void begin(Adafruit_SSD1306 &display);
  void showSplash(Adafruit_SSD1306 &display);
  void playStartupTone();
}

#endif // BOOTSEQUENCE_H
