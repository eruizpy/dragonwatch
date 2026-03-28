#include "clock_service.h"

#include <Arduino.h>

namespace dragonwatch::infrastructure {

uint32_t ArduinoClockService::nowMs() const { return millis(); }

}  // namespace dragonwatch::infrastructure
