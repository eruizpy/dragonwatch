#include "logger.h"

#include <Arduino.h>

namespace dragonwatch::platform {

void logInfo(const char* msg) {
  Serial.print("[INFO] ");
  Serial.println(msg);
}

void logWarn(const char* msg) {
  Serial.print("[WARN] ");
  Serial.println(msg);
}

void logError(const char* msg) {
  Serial.print("[ERROR] ");
  Serial.println(msg);
}

}  // namespace dragonwatch::platform
