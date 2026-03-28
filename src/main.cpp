#include <Arduino.h>

#include <string>
#include <vector>

#include "app_config.h"
#include "board_config.h"
#include "runtime_secrets.h"
#include "app/app_controller.h"
#include "app/scheduler.h"
#include "infrastructure/github_api_client.h"
#include "infrastructure/wifi_service.h"
#include "platform/board_support.h"
#include "platform/display_adapter.h"
#include "platform/logger.h"
#include "ui/screen_renderer.h"

namespace {

std::vector<std::string> configuredRepos() {
  std::vector<std::string> repos;
  if (std::string(DW_REPO_1).size() > 0) {
    repos.emplace_back(DW_REPO_1);
  }
  if (std::string(DW_REPO_2).size() > 0) {
    repos.emplace_back(DW_REPO_2);
  }
  if (std::string(DW_REPO_3).size() > 0) {
    repos.emplace_back(DW_REPO_3);
  }
  return repos;
}

}  // namespace

using namespace dragonwatch;

platform::TftEsPiDisplayAdapter display;
ui::ScreenRenderer renderer(display);
infrastructure::WifiService wifi(DW_WIFI_SSID, DW_WIFI_PASSWORD, AppConfig::wifiReconnectIntervalMs);
infrastructure::GitHubApiClient github(DW_GITHUB_TOKEN);
Scheduler scheduler({AppConfig::refreshIntervalMs, AppConfig::rotateIntervalMs, AppConfig::renderIntervalMs});
AppController app(wifi, github, renderer, scheduler, configuredRepos());

void setup() {
  Serial.begin(115200);
  platform::initBoard();

  display.begin();
  display.setRotation(BoardConfig::rotation);

  renderer.begin(BoardConfig::screenWidth, BoardConfig::screenHeight);

  app.begin();
  platform::logInfo("DragonWatch started");
}

void loop() { app.tick(millis()); }
