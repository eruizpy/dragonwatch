#pragma once

#include <cstddef>
#include <vector>

namespace dragonwatch::domain {

struct FloatPoint {
  float x;
  float y;
};

std::vector<FloatPoint> computeFloatingCommitPositions(std::size_t count,
                                                       float centerX,
                                                       float centerY,
                                                       float radius,
                                                       float phase);

}  // namespace dragonwatch::domain
