#include "github_json_parser.h"

#include <ArduinoJson.h>

#include "sha_utils.h"

namespace dragonwatch::domain {

std::vector<PullRequest> parsePullRequests(const std::string& repo, const std::string& payload) {
  std::vector<PullRequest> result;
  JsonDocument doc;
  const auto err = deserializeJson(doc, payload);
  if (err || !doc.is<JsonArray>()) {
    return result;
  }

  for (JsonVariant item : doc.as<JsonArray>()) {
    PullRequest pr;
    pr.repo = repo;
    pr.number = item["number"] | 0;
    pr.title = std::string(item["title"] | "");
    pr.author = std::string(item["user"]["login"] | "unknown");
    result.push_back(pr);
  }
  return result;
}

std::vector<CommitInfo> parseCommits(const std::string& payload) {
  std::vector<CommitInfo> commits;
  JsonDocument doc;
  const auto err = deserializeJson(doc, payload);
  if (err || !doc.is<JsonArray>()) {
    return commits;
  }

  for (JsonVariant item : doc.as<JsonArray>()) {
    CommitInfo commit;
    commit.sha = std::string(item["sha"] | "");
    commit.shortSha = normalizeSha(commit.sha);
    commits.push_back(commit);
  }

  return commits;
}

}  // namespace dragonwatch::domain
