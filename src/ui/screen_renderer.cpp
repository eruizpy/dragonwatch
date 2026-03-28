#include "screen_renderer.h"

#include <cmath>
#include <cstdio>

namespace dragonwatch::ui {

namespace {

float wrap01(float value) {
  const float floored = std::floor(value);
  return value - floored;
}

void drawKnife(dragonwatch::platform::IDisplayAdapter& display, int x, int y, const char* tag) {
  // Bigger blade + handle to make attack clearly visible.
  display.fillRect(x, y - 3, 20, 6, 0xC638);
  display.fillRect(x + 20, y - 4, 5, 8, 0xA145);
  display.fillRect(x - 5, y - 2, 5, 4, 0xFFFF);
  display.fillRect(x - 8, y - 1, 3, 2, 0xFFFF);
  display.drawString(tag, x + 28, y - 5, 1);
}

}  // namespace

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

  const bool attacking = vm.appState == dragonwatch::domain::AppState::READY;
  const int pomberoBaseX = layout_.dragonArea.w / 2;
  const int pomberoBaseY = layout_.dragonArea.y + layout_.dragonArea.h / 2;
  const int dodgeX = attacking ? static_cast<int>(std::sin(vm.phase * 7.0f) * 24.0f) : 0;
  const int dodgeY = attacking ? static_cast<int>(std::cos(vm.phase * 5.4f) * 6.0f) : 0;

  animator_.draw(display_, vm.dragonState, vm.frame, pomberoBaseX + dodgeX, pomberoBaseY + dodgeY);

  const std::size_t knivesCount = vm.commits.empty() ? 1 : vm.commits.size();
  const int laneSpacing = 22;
  const int centerLaneY = pomberoBaseY;
  const int attackStartX = layout_.commitArea.x + layout_.commitArea.w - 22;
  const int attackEndX = pomberoBaseX - 12;
  const int attackDistance = attackStartX - attackEndX;

  for (std::size_t i = 0; i < knivesCount; ++i) {
    const float t = wrap01(vm.phase * 0.26f + static_cast<float>(i) * 0.17f);
    const int x = attackStartX - static_cast<int>(t * static_cast<float>(attackDistance));
    const int yOffset = (static_cast<int>(i) - static_cast<int>(knivesCount) / 2) * laneSpacing;
    const int yJitter = attacking ? static_cast<int>(std::sin(vm.phase * 9.0f + static_cast<float>(i)) * 4.0f) : 0;
    const int y = centerLaneY + yOffset + yJitter;
    const char* tag = vm.commits.empty() ? "PR" : vm.commits[i].shortSha.c_str();
    drawKnife(display_, x, y, tag);
  }

  const uint16_t barColor = statusColor(vm.appState);
  display_.fillRect(layout_.statusBar.x, layout_.statusBar.y, layout_.statusBar.w, layout_.statusBar.h, barColor);
  display_.setTextColor(0xFFFF, barColor);
  display_.drawString(statusText(vm.appState), 6, layout_.statusBar.y + 6, 2);
}

}  // namespace dragonwatch::ui
