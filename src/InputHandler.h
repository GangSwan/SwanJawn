#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <Arduino.h>
#include <RotaryEncoder.h>
#include "Config.h"
#include "DisplayUI.h"
#include "SoundFX.h"

namespace InputHandler {
  void begin();
  void update();
}

#endif // INPUTHANDLER_H
