#pragma once

#include <string>
#include <vector>

#include "../domain/models/pull_request.h"
#include "json_parser.h"

namespace dragonwatch::infrastructure {

enum class FetchStatus {
  Ok,
  NetworkError,
  ApiError,
  RateLimit,
};

struct FetchResult {
  FetchStatus status = FetchStatus::ApiError;
  std::vector<dragonwatch::domain::PullRequest> pullRequests;
  int httpCode = 0;
};

struct CommitsFetchResult {
  FetchStatus status = FetchStatus::ApiError;
  std::vector<dragonwatch::domain::CommitInfo> commits;
  int httpCode = 0;
};

class IGitHubApiClient {
 public:
  virtual ~IGitHubApiClient() = default;
  virtual FetchResult fetchOpenPullRequests(const std::vector<std::string>& repos) = 0;
  virtual CommitsFetchResult fetchPullRequestCommits(const std::string& repo, int prNumber) = 0;
};

class GitHubApiClient final : public IGitHubApiClient {
 public:
  explicit GitHubApiClient(const char* token);

  FetchResult fetchOpenPullRequests(const std::vector<std::string>& repos) override;
  CommitsFetchResult fetchPullRequestCommits(const std::string& repo, int prNumber) override;

 private:
  FetchResult mapHttpError(int code) const;
  std::string buildPullsUrl(const std::string& repo) const;
  std::string buildCommitsUrl(const std::string& repo, int prNumber) const;

  const char* token_;
  JsonParser parser_;
};

}  // namespace dragonwatch::infrastructure
