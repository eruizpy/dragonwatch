#pragma once

#include <array>
#include <cstddef>

namespace dragonwatch {

struct AppConfig {
  static constexpr unsigned long refreshIntervalMs = 15000;
  static constexpr unsigned long rotateIntervalMs = 6000;
  static constexpr unsigned long renderIntervalMs = 50;
  static constexpr unsigned long wifiReconnectIntervalMs = 5000;
  static constexpr unsigned long maxBackoffMs = 120000;
  static constexpr std::size_t maxRepos = 3;
  static constexpr std::size_t maxCommitsOnScreen = 6;
};

}  // namespace dragonwatch
