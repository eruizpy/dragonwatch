#pragma once

#include <optional>
#include <vector>

#include "../models/pull_request.h"

namespace dragonwatch::domain {

std::optional<std::size_t> selectInitialPrIndex(const std::vector<PullRequest>& prs);
std::optional<std::size_t> selectNextPrIndex(const std::vector<PullRequest>& prs, std::size_t currentIndex);

}  // namespace dragonwatch::domain
