#include "screen_renderer.h"

#include <cstdio>

#include "../domain/animation/floating_commits.h"

namespace dragonwatch::ui {

ScreenRenderer::ScreenRenderer(dragonwatch::platform::IDisplayAdapter& display) : display_(display) {}

void ScreenRenderer::begin(int width, int height) {
  layout_ = dragonwatch::domain::computeLayout(width, height);
  display_.fillScreen(0x0000);
  initialized_ = true;
}

const char* ScreenRenderer::statusText(dragonwatch::domain::AppState state) const {
  switch (state) {
    case dragonwatch::domain::AppState::LOADING:
      return "Loading";
    case dragonwatch::domain::AppState::READY:
      return "Ready";
    case dragonwatch::domain::AppState::NO_PRS:
      return "No PRs";
    case dragonwatch::domain::AppState::RECONNECTING:
      return "Reconnecting";
    case dragonwatch::domain::AppState::ERROR_NETWORK:
      return "Network error";
    case dragonwatch::domain::AppState::ERROR_API:
      return "API error";
    case dragonwatch::domain::AppState::RATE_LIMIT:
      return "Rate limited";
    default:
      return "Unknown";
  }
}

uint16_t ScreenRenderer::statusColor(dragonwatch::domain::AppState state) const {
  switch (state) {
    case dragonwatch::domain::AppState::READY:
      return 0x07E0;
    case dragonwatch::domain::AppState::RATE_LIMIT:
      return 0xFD20;
    case dragonwatch::domain::AppState::ERROR_NETWORK:
    case dragonwatch::domain::AppState::ERROR_API:
      return 0xF800;
    default:
      return 0x7BEF;
  }
}

void ScreenRenderer::render(const ScreenViewModel& vm) {
  if (!initialized_) {
    return;
  }

  display_.fillRect(layout_.header.x, layout_.header.y, layout_.header.w, layout_.header.h, 0x0000);
  display_.fillRect(layout_.dragonArea.x, layout_.dragonArea.y, layout_.dragonArea.w, layout_.dragonArea.h, 0x0000);
  display_.fillRect(layout_.commitArea.x, layout_.commitArea.y, layout_.commitArea.w, layout_.commitArea.h, 0x0000);

  display_.setTextColor(0xFFFF, 0x0000);
  char line1[64];
  std::snprintf(line1, sizeof(line1), "Repo: %s", vm.repo.c_str());
  display_.drawString(line1, 4, 4, 2);

  char line2[64];
  std::snprintf(line2, sizeof(line2), "PR #%d - %s", vm.prNumber, vm.prAuthor.c_str());
  display_.drawString(line2, 4, 22, 2);

  char line3[72];
  std::snprintf(line3, sizeof(line3), "%s", vm.prTitle.c_str());
  display_.drawString(line3, 4, 40, 2);

  animator_.draw(display_, vm.dragonState, vm.frame, layout_.dragonArea.w / 2, layout_.dragonArea.y + layout_.dragonArea.h / 2);

  const auto floating = dragonwatch::domain::computeFloatingCommitPositions(
      vm.commits.size(),
      static_cast<float>(layout_.commitArea.x + layout_.commitArea.w / 2),
      static_cast<float>(layout_.commitArea.y + layout_.commitArea.h / 2),
      44.0f,
      vm.phase);

  for (std::size_t i = 0; i < floating.size(); ++i) {
    display_.fillRect(static_cast<int>(floating[i].x) - 18, static_cast<int>(floating[i].y) - 8, 36, 14, 0x0010);
    display_.drawRect(static_cast<int>(floating[i].x) - 18, static_cast<int>(floating[i].y) - 8, 36, 14, 0x39E7);
    display_.drawString(vm.commits[i].shortSha.c_str(), static_cast<int>(floating[i].x) - 16, static_cast<int>(floating[i].y) - 7,
                        1);
  }

  const uint16_t barColor = statusColor(vm.appState);
  display_.fillRect(layout_.statusBar.x, layout_.statusBar.y, layout_.statusBar.w, layout_.statusBar.h, barColor);
  display_.setTextColor(0xFFFF, barColor);
  display_.drawString(statusText(vm.appState), 6, layout_.statusBar.y + 6, 2);
}

}  // namespace dragonwatch::ui
