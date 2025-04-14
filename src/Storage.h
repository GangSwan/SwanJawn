#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

namespace Storage {
  bool begin();                        // Initialize SD card
  bool logBoot(const char* entry);  // Write to bootlog.txt
  bool logData(const char* entry);  // Write to log.txt
  bool logMidiEvent(const char* line);

  
}



#endif // STORAGE_H
