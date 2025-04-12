#include "Storage.h"
#include <SD.h>
#include <SPI.h>

#define SD_CS 5  // Chip select pin

namespace Storage {
  bool begin() {
    return SD.begin(SD_CS);
  }

  bool logBoot(const String& entry) {
    File file = SD.open("/bootlog.txt", FILE_WRITE);
    if (file) {
      file.println(entry);
      file.close();
      return true;
    }
    return false;
  }

  bool logData(const String& entry) {
    File file = SD.open("/log.txt", FILE_WRITE);
    if (file) {
      file.println(entry);
      file.close();
      return true;
    }
    return false;
  }
}
