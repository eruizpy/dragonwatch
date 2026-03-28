#pragma once

#include <cstdint>

namespace dragonwatch::domain {

class BackoffPolicy {
 public:
  BackoffPolicy(uint32_t baseDelayMs, uint32_t maxDelayMs);
  uint32_t currentDelay() const;
  void onFailure();
  void onSuccess();

 private:
  uint32_t baseDelayMs_;
  uint32_t maxDelayMs_;
  uint32_t currentDelayMs_;
};

}  // namespace dragonwatch::domain
