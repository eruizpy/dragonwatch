#pragma once

#include <cstdint>

namespace dragonwatch::infrastructure {

class IWifiService {
 public:
  virtual ~IWifiService() = default;
  virtual void begin() = 0;
  virtual void tick(uint32_t nowMs) = 0;
  virtual bool isConnected() const = 0;
};

class WifiService final : public IWifiService {
 public:
  WifiService(const char* ssid, const char* password, uint32_t reconnectIntervalMs);

  void begin() override;
  void tick(uint32_t nowMs) override;
  bool isConnected() const override;

 private:
  const char* ssid_;
  const char* password_;
  uint32_t reconnectIntervalMs_;
  uint32_t lastReconnectAttemptMs_;
};

}  // namespace dragonwatch::infrastructure
