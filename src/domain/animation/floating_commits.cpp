#include "floating_commits.h"

#include <cmath>

namespace dragonwatch::domain {

std::vector<FloatPoint> computeFloatingCommitPositions(std::size_t count,
                                                       float centerX,
                                                       float centerY,
                                                       float radius,
                                                       float phase) {
  constexpr float kTwoPi = 6.28318530718f;
  std::vector<FloatPoint> out;
  if (count == 0) {
    return out;
  }

  out.reserve(count);
  const float step = kTwoPi / static_cast<float>(count);
  for (std::size_t i = 0; i < count; ++i) {
    const float angle = phase + step * static_cast<float>(i);
    out.push_back({centerX + std::cos(angle) * radius, centerY + std::sin(angle) * radius * 0.6f});
  }
  return out;
}

}  // namespace dragonwatch::domain
