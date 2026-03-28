#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../domain/models/app_state.h"
#include "../domain/models/pull_request.h"
#include "../domain/services/app_state_machine.h"
#include "../domain/services/backoff_policy.h"
#include "../domain/services/pr_selector.h"
#include "../infrastructure/github_api_client.h"
#include "../infrastructure/wifi_service.h"
#include "../ui/screen_renderer.h"
#include "scheduler.h"

namespace dragonwatch {

class AppController {
 public:
  AppController(infrastructure::IWifiService& wifi,
                infrastructure::IGitHubApiClient& github,
                ui::ScreenRenderer& renderer,
                Scheduler scheduler,
                std::vector<std::string> repos);

  void begin();
  void tick(uint32_t nowMs);

 private:
  void refreshData();
  void refreshActiveCommits();
  void rotateData();
  ui::ScreenViewModel buildViewModel() const;

  infrastructure::IWifiService& wifi_;
  infrastructure::IGitHubApiClient& github_;
  ui::ScreenRenderer& renderer_;
  Scheduler scheduler_;
  std::vector<std::string> repos_;

  domain::AppState appState_;
  std::vector<domain::PullRequest> prs_;
  std::vector<domain::CommitInfo> activeCommits_;
  std::size_t activeIndex_;
  domain::BackoffPolicy backoff_;
  uint32_t frame_;
  float phase_;
};

}  // namespace dragonwatch
