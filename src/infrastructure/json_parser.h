#pragma once

#include <string>
#include <vector>

#include "../domain/models/commit_info.h"
#include "../domain/models/pull_request.h"

namespace dragonwatch::infrastructure {

class JsonParser {
 public:
  std::vector<domain::PullRequest> parsePullRequests(const std::string& repo, const std::string& payload) const;
  std::vector<domain::CommitInfo> parseCommits(const std::string& payload) const;
};

}  // namespace dragonwatch::infrastructure
