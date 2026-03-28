#pragma once

#include <string>

namespace dragonwatch::domain {

struct CommitInfo {
  std::string sha;
  std::string shortSha;
};

}  // namespace dragonwatch::domain
