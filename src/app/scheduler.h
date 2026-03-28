#pragma once

#include <cstdint>

namespace dragonwatch {

struct SchedulerConfig {
  uint32_t refreshIntervalMs;
  uint32_t rotateIntervalMs;
  uint32_t renderIntervalMs;
};

class Scheduler {
 public:
  explicit Scheduler(SchedulerConfig config);

  bool shouldRefresh(uint32_t nowMs);
  bool shouldRotate(uint32_t nowMs);
  bool shouldRender(uint32_t nowMs);

 private:
  SchedulerConfig config_;
  uint32_t lastRefresh_;
  uint32_t lastRotate_;
  uint32_t lastRender_;
  bool refreshInitialized_;
  bool rotateInitialized_;
  bool renderInitialized_;
};

}  // namespace dragonwatch
