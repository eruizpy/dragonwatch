#include "app_controller.h"

#include <Arduino.h>
#include <utility>

namespace dragonwatch {

AppController::AppController(infrastructure::IWifiService& wifi,
                             infrastructure::IGitHubApiClient& github,
                             ui::ScreenRenderer& renderer,
                             Scheduler scheduler,
                             std::vector<std::string> repos)
    : wifi_(wifi),
      github_(github),
      renderer_(renderer),
      scheduler_(scheduler),
      repos_(std::move(repos)),
      appState_(domain::AppState::LOADING),
      activeIndex_(0),
      backoff_(5000, 120000),
      frame_(0),
      phase_(0.0f) {}

void AppController::begin() {
  wifi_.begin();
  appState_ = domain::transitionAppState(appState_, domain::DataEvent::Start);
  Serial.println("[DragonWatch] App started");
}

void AppController::tick(uint32_t nowMs) {
  wifi_.tick(nowMs);

  if (!wifi_.isConnected()) {
    appState_ = domain::transitionAppState(appState_, domain::DataEvent::WifiLost);
  } else if (appState_ == domain::AppState::RECONNECTING) {
    appState_ = domain::transitionAppState(appState_, domain::DataEvent::WifiRecovered);
  }

  if (wifi_.isConnected() && scheduler_.shouldRefresh(nowMs)) {
    refreshData();
  }

  if (appState_ == domain::AppState::READY && scheduler_.shouldRotate(nowMs)) {
    rotateData();
  }

  if (scheduler_.shouldRender(nowMs)) {
    frame_++;
    phase_ += 0.07f;
    renderer_.render(buildViewModel());
  }
}

void AppController::refreshData() {
  Serial.println("[DragonWatch] Refreshing GitHub data...");
  const auto result = github_.fetchOpenPullRequests(repos_);
  switch (result.status) {
    case infrastructure::FetchStatus::Ok:
      prs_ = result.pullRequests;
      Serial.printf("[DragonWatch] Fetch OK. PR count=%u\n", static_cast<unsigned>(prs_.size()));
      if (prs_.empty()) {
        activeCommits_.clear();
        appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchSuccessNoData);
      } else {
        appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchSuccessWithData);
        if (activeIndex_ >= prs_.size()) {
          activeIndex_ = 0;
        }
        refreshActiveCommits();
      }
      backoff_.onSuccess();
      break;
    case infrastructure::FetchStatus::NetworkError:
      Serial.printf("[DragonWatch] Network error. http=%d\n", result.httpCode);
      appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchNetworkError);
      backoff_.onFailure();
      break;
    case infrastructure::FetchStatus::ApiError:
      Serial.printf("[DragonWatch] API error. http=%d\n", result.httpCode);
      appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchApiError);
      backoff_.onFailure();
      break;
    case infrastructure::FetchStatus::RateLimit:
      Serial.printf("[DragonWatch] Rate limited. http=%d\n", result.httpCode);
      appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchRateLimit);
      backoff_.onFailure();
      break;
    default:
      break;
  }
}

void AppController::rotateData() {
  auto next = domain::selectNextPrIndex(prs_, activeIndex_);
  if (next.has_value()) {
    activeIndex_ = next.value();
    refreshActiveCommits();
  }
}

void AppController::refreshActiveCommits() {
  if (prs_.empty() || activeIndex_ >= prs_.size()) {
    activeCommits_.clear();
    return;
  }

  const auto& pr = prs_[activeIndex_];
  const auto commitsResult = github_.fetchPullRequestCommits(pr.repo, pr.number);
  if (commitsResult.status == infrastructure::FetchStatus::Ok) {
    activeCommits_ = commitsResult.commits;
    Serial.printf("[DragonWatch] Active PR commits=%u\n", static_cast<unsigned>(activeCommits_.size()));
  } else {
    Serial.printf("[DragonWatch] Commits fetch fail. http=%d\n", commitsResult.httpCode);
  }
}

ui::ScreenViewModel AppController::buildViewModel() const {
  ui::ScreenViewModel vm;
  vm.appState = appState_;
  vm.dragonState = domain::resolveDragonState(appState_);
  vm.frame = frame_;
  vm.phase = phase_;
  vm.openPrCount = static_cast<int>(prs_.size());

  if (!prs_.empty() && activeIndex_ < prs_.size()) {
    const auto& pr = prs_[activeIndex_];
    vm.repo = pr.repo;
    vm.prNumber = pr.number;
    vm.prTitle = pr.title;
    vm.prAuthor = pr.author;
    vm.commits = &activeCommits_;
  }

  return vm;
}

}  // namespace dragonwatch
