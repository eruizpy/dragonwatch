#include "pr_selector.h"

namespace dragonwatch::domain {

std::optional<std::size_t> selectInitialPrIndex(const std::vector<PullRequest>& prs) {
  if (prs.empty()) {
    return std::nullopt;
  }
  return 0;
}

std::optional<std::size_t> selectNextPrIndex(const std::vector<PullRequest>& prs, std::size_t currentIndex) {
  if (prs.empty()) {
    return std::nullopt;
  }
  return (currentIndex + 1) % prs.size();
}

}  // namespace dragonwatch::domain
