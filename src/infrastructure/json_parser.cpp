#include "json_parser.h"

#include "../domain/services/github_json_parser.h"

namespace dragonwatch::infrastructure {

std::vector<domain::PullRequest> JsonParser::parsePullRequests(const std::string& repo, const std::string& payload) const {
  return domain::parsePullRequests(repo, payload);
}

std::vector<domain::CommitInfo> JsonParser::parseCommits(const std::string& payload) const {
  return domain::parseCommits(payload);
}

}  // namespace dragonwatch::infrastructure
