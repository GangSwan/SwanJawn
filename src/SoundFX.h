#ifndef SOUNDFX_H
#define SOUNDFX_H

#include <Arduino.h>
#include "Config.h"
#include "DisplayUI.h" // so we can check if Silent Mode is active

namespace SoundFX {
  void playGiornosTheme();
  void playSignalTone(int rssi);
}

#endif // SOUNDFX_H


// Implementation moved to SoundFX.cpp

