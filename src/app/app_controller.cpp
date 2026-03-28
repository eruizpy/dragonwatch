#include "app_controller.h"

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
  const auto result = github_.fetchOpenPullRequests(repos_);
  switch (result.status) {
    case infrastructure::FetchStatus::Ok:
      prs_ = result.pullRequests;
      if (prs_.empty()) {
        appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchSuccessNoData);
      } else {
        appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchSuccessWithData);
      }
      backoff_.onSuccess();
      if (activeIndex_ >= prs_.size()) {
        activeIndex_ = 0;
      }
      break;
    case infrastructure::FetchStatus::NetworkError:
      appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchNetworkError);
      backoff_.onFailure();
      break;
    case infrastructure::FetchStatus::ApiError:
      appState_ = domain::transitionAppState(appState_, domain::DataEvent::FetchApiError);
      backoff_.onFailure();
      break;
    case infrastructure::FetchStatus::RateLimit:
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
  }
}

ui::ScreenViewModel AppController::buildViewModel() const {
  ui::ScreenViewModel vm;
  vm.appState = appState_;
  vm.dragonState = domain::resolveDragonState(appState_);
  vm.frame = frame_;
  vm.phase = phase_;

  if (!prs_.empty() && activeIndex_ < prs_.size()) {
    const auto& pr = prs_[activeIndex_];
    vm.repo = pr.repo;
    vm.prNumber = pr.number;
    vm.prTitle = pr.title;
    vm.prAuthor = pr.author;
    vm.commits = pr.commits;
  }

  return vm;
}

}  // namespace dragonwatch
