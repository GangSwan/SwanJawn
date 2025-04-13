#ifndef SNIFFER_H
#define SNIFFER_H

#include <Arduino.h>



namespace Sniffer {
  int getCurrentChannel();
  int getPacketCount();
  String getLastMAC();
  int getLastRSSI();

  void begin();
  void loop();
  void stop();
}

#endif
