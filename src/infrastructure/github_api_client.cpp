#include "github_api_client.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

namespace dragonwatch::infrastructure {

GitHubApiClient::GitHubApiClient(const char* token) : token_(token) {}

FetchResult GitHubApiClient::mapHttpError(int code) const {
  if (code == 403) {
    return {FetchStatus::RateLimit, {}, code};
  }
  if (code <= 0) {
    return {FetchStatus::NetworkError, {}, code};
  }
  return {FetchStatus::ApiError, {}, code};
}

std::string GitHubApiClient::buildPullsUrl(const std::string& repo) const {
  return "https://api.github.com/repos/" + repo + "/pulls?state=open&per_page=5";
}

std::string GitHubApiClient::buildCommitsUrl(const std::string& repo, int prNumber) const {
  return "https://api.github.com/repos/" + repo + "/pulls/" + std::to_string(prNumber) + "/commits?per_page=10";
}

FetchResult GitHubApiClient::fetchOpenPullRequests(const std::vector<std::string>& repos) {
  FetchResult result;
  result.status = FetchStatus::Ok;

  WiFiClientSecure secureClient;
  secureClient.setInsecure();

  for (const auto& repo : repos) {
    if (repo.empty()) {
      continue;
    }

    HTTPClient http;
    const auto url = buildPullsUrl(repo);
    if (!http.begin(secureClient, url.c_str())) {
      return {FetchStatus::NetworkError, {}, -1};
    }

    http.addHeader("Accept", "application/vnd.github+json");
    http.addHeader("User-Agent", "dragonwatch/0.1");
    http.addHeader("Authorization", std::string("Bearer ").append(token_).c_str());

    const int code = http.GET();
    if (code != 200) {
      http.end();
      return mapHttpError(code);
    }

    const std::string payload = http.getString().c_str();
    http.end();

    auto prs = parser_.parsePullRequests(repo, payload);
    for (auto& pr : prs) {
      HTTPClient commitsHttp;
      const auto commitsUrl = buildCommitsUrl(repo, pr.number);
      if (!commitsHttp.begin(secureClient, commitsUrl.c_str())) {
        return {FetchStatus::NetworkError, {}, -2};
      }

      commitsHttp.addHeader("Accept", "application/vnd.github+json");
      commitsHttp.addHeader("User-Agent", "dragonwatch/0.1");
      commitsHttp.addHeader("Authorization", std::string("Bearer ").append(token_).c_str());

      const int commitsCode = commitsHttp.GET();
      if (commitsCode != 200) {
        commitsHttp.end();
        return mapHttpError(commitsCode);
      }

      const std::string commitsPayload = commitsHttp.getString().c_str();
      commitsHttp.end();

      pr.commits = parser_.parseCommits(commitsPayload);
      result.pullRequests.push_back(pr);
    }
  }

  return result;
}

}  // namespace dragonwatch::infrastructure
