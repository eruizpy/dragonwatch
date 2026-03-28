#pragma once

#include <string>
#include <vector>

#include "../models/pull_request.h"

namespace dragonwatch::domain {

std::vector<PullRequest> parsePullRequests(const std::string& repo, const std::string& payload);
std::vector<CommitInfo> parseCommits(const std::string& payload);

}  // namespace dragonwatch::domain
