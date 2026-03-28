#include "wifi_service.h"

#include <WiFi.h>

namespace dragonwatch::infrastructure {

WifiService::WifiService(const char* ssid, const char* password, uint32_t reconnectIntervalMs)
    : ssid_(ssid),
      password_(password),
      reconnectIntervalMs_(reconnectIntervalMs),
      lastReconnectAttemptMs_(0) {}

void WifiService::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_, password_);
}

void WifiService::tick(uint32_t nowMs) {
  if (isConnected()) {
    return;
  }

  if (lastReconnectAttemptMs_ == 0 || nowMs - lastReconnectAttemptMs_ >= reconnectIntervalMs_) {
    WiFi.disconnect();
    WiFi.begin(ssid_, password_);
    lastReconnectAttemptMs_ = nowMs;
  }
}

bool WifiService::isConnected() const { return WiFi.status() == WL_CONNECTED; }

}  // namespace dragonwatch::infrastructure
