#pragma once

#include <string>
#include <vector>

#include "commit_info.h"

namespace dragonwatch::domain {

struct PullRequest {
  int number = 0;
  std::string repo;
  std::string title;
  std::string author;
  std::vector<CommitInfo> commits;
};

}  // namespace dragonwatch::domain
