#include "Sniffer.h"
#include "Storage.h"
#include <esp_wifi.h>
#include <WiFi.h>



static bool sniffing = false;

static void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;

  const wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*) buf;
  const uint8_t* addr = pkt->payload;

  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
    addr[10], addr[11], addr[12], addr[13], addr[14], addr[15]);

  int rssi = pkt->rx_ctrl.rssi;

  String logEntry = "MAC: " + String(macStr) + " RSSI: " + String(rssi);
  Serial.println(logEntry);
  Storage::logData(logEntry);
}

void Sniffer::begin() {
  if (sniffing) return;

  WiFi.mode(WIFI_MODE_NULL); // Disable station + AP
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);
  sniffing = true;

  Serial.println("[SNIFFER] Sniffing enabled.");
}

void Sniffer::loop() {
  // Not needed unless we add auto-channel switching
}

void Sniffer::stop() {
  if (!sniffing) return;

  esp_wifi_set_promiscuous(false);
  sniffing = false;

  Serial.println("[SNIFFER] Sniffing stopped.");
}
