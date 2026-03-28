#pragma once

#include <cstdint>

namespace dragonwatch::infrastructure {

class ClockService {
 public:
  virtual ~ClockService() = default;
  virtual uint32_t nowMs() const = 0;
};

class ArduinoClockService final : public ClockService {
 public:
  uint32_t nowMs() const override;
};

}  // namespace dragonwatch::infrastructure
