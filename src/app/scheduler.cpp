#include "scheduler.h"

namespace dragonwatch {

Scheduler::Scheduler(SchedulerConfig config)
    : config_(config),
      lastRefresh_(0),
      lastRotate_(0),
      lastRender_(0),
      refreshInitialized_(false),
      rotateInitialized_(false),
      renderInitialized_(false) {}

bool Scheduler::shouldRefresh(uint32_t nowMs) {
  if (!refreshInitialized_ || nowMs - lastRefresh_ >= config_.refreshIntervalMs) {
    lastRefresh_ = nowMs;
    refreshInitialized_ = true;
    return true;
  }
  return false;
}

bool Scheduler::shouldRotate(uint32_t nowMs) {
  if (!rotateInitialized_ || nowMs - lastRotate_ >= config_.rotateIntervalMs) {
    lastRotate_ = nowMs;
    rotateInitialized_ = true;
    return true;
  }
  return false;
}

bool Scheduler::shouldRender(uint32_t nowMs) {
  if (!renderInitialized_ || nowMs - lastRender_ >= config_.renderIntervalMs) {
    lastRender_ = nowMs;
    renderInitialized_ = true;
    return true;
  }
  return false;
}

}  // namespace dragonwatch
