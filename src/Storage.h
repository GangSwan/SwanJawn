#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

namespace Storage {
  bool begin();                        // Initialize SD card
  bool logBoot(const String& entry);  // Write to bootlog.txt
  bool logData(const String& entry);  // Write to log.txt
}

#endif // STORAGE_H
