#include "backoff_policy.h"

namespace dragonwatch::domain {

BackoffPolicy::BackoffPolicy(uint32_t baseDelayMs, uint32_t maxDelayMs)
    : baseDelayMs_(baseDelayMs), maxDelayMs_(maxDelayMs), currentDelayMs_(baseDelayMs) {}

uint32_t BackoffPolicy::currentDelay() const { return currentDelayMs_; }

void BackoffPolicy::onFailure() {
  const uint32_t doubled = currentDelayMs_ * 2;
  currentDelayMs_ = doubled > maxDelayMs_ ? maxDelayMs_ : doubled;
}

void BackoffPolicy::onSuccess() { currentDelayMs_ = baseDelayMs_; }

}  // namespace dragonwatch::domain
