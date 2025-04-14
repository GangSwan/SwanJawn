#include "Storage.h"
#include <SD.h>
#include <SPI.h>

#define SD_CS 5  // Chip select pin

namespace Storage {
  bool begin() {
    return SD.begin(SD_CS);
  }

  bool logBoot(const char* entry) {
    File file = SD.open("/bootlog.txt", FILE_WRITE);
    if (file) {
      file.println(entry);
      file.close();
      return true;
    }
    return false;
  }

  bool logData(const char* entry) {
    File file = SD.open("/log.txt", FILE_WRITE);
    if (file) {
      file.println(entry);
      file.close();
      return true;
    }
    return false;
  }

  bool logMidiEvent(const char* line) {
    File f = SD.open("/midi_log.csv", FILE_WRITE);
    if (!f) return false;
    f.println(line);
    f.close();
    return true;
  }
  
}
