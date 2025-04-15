#include "Sniffer.h"
#include "Storage.h"
#include <esp_wifi.h>
#include <WiFi.h>
#include "DisplayUI.h"
#include "SoundFX.h"


static bool sniffing = false;
static int packetCount = 0;
static String lastMAC = "";
static int lastRSSI = 0;
static int currentChannel = 1; // current Wi-Fi channel
static const int rssiHistorySize = 128; // one pixel per sample
static int rssiHistory[rssiHistorySize];
static int rssiIndex = 0;
static char queuedMidiLine[64] = ""; // only holds one line at a time
static bool midiLineReady = false;



static void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;

  const wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*) buf;
  const uint8_t* addr = pkt->payload;

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           addr[10], addr[11], addr[12], addr[13], addr[14], addr[15]);

  int rssi = pkt->rx_ctrl.rssi;
  lastMAC = String(macStr);
  lastRSSI = rssi;

  rssiHistory[rssiIndex] = rssi;
  rssiIndex = (rssiIndex + 1) % rssiHistorySize;
  packetCount++;

  // SD log every 5 packets
  static int logSkipCounter = 0;
  if (++logSkipCounter % 5 == 0) {
    char logBuf[64];
    snprintf(logBuf, sizeof(logBuf), "MAC: %s RSSI: %d", macStr, rssi);
    Storage::logData(logBuf);
  }

  // Debug (optional)
  Serial.println("Free heap: " + String(ESP.getFreeHeap()));

  // Sound feedback
  SoundFX::playSignalTone(rssi);

  // Log MIDI pitch data
  int clamped = constrain(rssi, -95, -30);
  int note = map(clamped, -95, -30, 60, 84); // MIDI C4–C6
  char midiLine[64];
  snprintf(midiLine, sizeof(midiLine), "%lu,%d,30", millis(), note);
 
  Serial.println(midiLine);  // for the Python bridge

  
  if (!midiLineReady) {
    strncpy(queuedMidiLine, midiLine, sizeof(queuedMidiLine));
    midiLineReady = true;
  }
  
}



void Sniffer::begin() {
  if (sniffing) return;

  WiFi.mode(WIFI_MODE_NULL);
  esp_wifi_stop();
  esp_wifi_deinit();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();

  esp_err_t err;
  err = esp_wifi_set_promiscuous(true);
  Serial.println("[SNIFFER] Promiscuous enable result: " + String(err));

  err = esp_wifi_set_promiscuous_rx_cb(snifferCallback);
  Serial.println("[SNIFFER] Callback set result: " + String(err));

  if (DisplayUI::modeSelection == 1) { // Channel mode
    currentChannel = DisplayUI::channelSelection + 1;
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
    Serial.println("[SNIFFER] Locked to channel " + String(currentChannel));
  } else {
    // Scan mode - start on current channel, will auto hop in loop
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
  }
  
  Serial.println("[SNIFFER] Channel set to CH " + String(currentChannel) + ", result: " + String(err));

  char bootMsg[48];
  snprintf(bootMsg, sizeof(bootMsg), "DEBUG: Sniffer started on CH %d", currentChannel);
  Storage::logData(bootMsg);

  Serial.println("[SNIFFER] Promiscuous mode started.");
  sniffing = true;
}

void Sniffer::loop() {
  static uint32_t lastHopTime = 0;

  if (midiLineReady) {
    Storage::logMidiEvent(queuedMidiLine);
    midiLineReady = false;
  }
  

  if (!sniffing) return;

  if (DisplayUI::modeSelection == 0) { // Scan mode
    const int scanSpeeds[] = { 1000, 2000, 3000, 5000, 8000, 10000 };
    int interval = scanSpeeds[DisplayUI::speedSelection];
  
    if (millis() - lastHopTime > interval) {
      currentChannel++;
      if (currentChannel > 11) currentChannel = 1;
      esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
      Serial.println("[SNIFFER] Switched to channel " + String(currentChannel));
      lastHopTime = millis();
    }
  }
  
}

void Sniffer::stop() {
  if (!sniffing) return;

  esp_wifi_set_promiscuous(false);
  sniffing = false;

  packetCount = 0;
  lastMAC = "";
  lastRSSI = 0;

  Serial.println("[SNIFFER] Sniffing stopped.");
}

int Sniffer::getCurrentChannel() {
  return currentChannel;
}

namespace Sniffer {
  int getPacketCount() {
    return packetCount;
  }

  String getLastMAC() {
    return lastMAC;
  }

  int getLastRSSI() {
    return lastRSSI;
  }
  
  const int* getRSSIHistory() {
    return rssiHistory;
  }
  
}
