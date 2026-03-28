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
  (void)tag;
  display.fillRect(x, y - 2, 16, 4, 0xC638);
  display.fillRect(x + 16, y - 3, 4, 6, 0xA145);
  display.fillRect(x - 4, y - 1, 4, 2, 0xFFFF);
}

void drawCutMark(dragonwatch::platform::IDisplayAdapter& display, int centerX, int centerY, uint32_t frame) {
  const bool alt = (frame / 3) % 2 == 0;
  const uint16_t c = alt ? 0xF800 : 0xFFE0;
  display.fillRect(centerX + 7, centerY - 8, 2, 8, c);
  display.fillRect(centerX + 9, centerY - 10, 2, 8, c);
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
  static int lastPrNumber = -1;
  static std::string lastRepo;

  if (!initialized_) {
    return;
  }

  const bool headerChanged = (vm.prNumber != lastPrNumber) || (vm.repo != lastRepo);
  if (headerChanged) {
    display_.fillRect(layout_.header.x, layout_.header.y, layout_.header.w, layout_.header.h, 0x0000);
  }
  display_.fillRect(layout_.dragonArea.x, layout_.dragonArea.y, layout_.dragonArea.w, layout_.dragonArea.h, 0x0000);
  display_.fillRect(layout_.commitArea.x, layout_.commitArea.y, layout_.commitArea.w, layout_.commitArea.h, 0x0000);

  if (headerChanged) {
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

    lastPrNumber = vm.prNumber;
    lastRepo = vm.repo;
  }

  const bool attacking = vm.appState == dragonwatch::domain::AppState::READY;
  const int pomberoBaseX = layout_.dragonArea.w / 2;
  const int pomberoBaseY = layout_.dragonArea.y + layout_.dragonArea.h / 2;
  int pomberoX = pomberoBaseX;
  int pomberoY = pomberoBaseY;

  if (attacking) {
    const int runLeft = layout_.dragonArea.x + 36;
    const int runRight = layout_.dragonArea.w - 44;
    const float runT = wrap01(vm.phase * 0.40f);
    pomberoX = runRight - static_cast<int>(runT * static_cast<float>(runRight - runLeft));
    pomberoY = pomberoBaseY + static_cast<int>(std::sin(vm.phase * 8.0f) * 5.0f);
  }

  animator_.draw(display_, vm.dragonState, vm.frame, pomberoX, pomberoY);

  const std::size_t knivesCountRaw = vm.openPrCount > 0 ? static_cast<std::size_t>(vm.openPrCount) : 1;
  const std::size_t knivesCount = knivesCountRaw > 6 ? 6 : knivesCountRaw;
  const int laneSpacing = knivesCount >= 5 ? 14 : (knivesCount >= 4 ? 18 : 24);
  const int centerLaneY = pomberoY;
  const int attackStartX = layout_.commitArea.x + layout_.commitArea.w - 22;
  const int attackEndX = pomberoX - 8;
  const int attackDistance = attackStartX - attackEndX;
  bool gotHit = false;

  for (std::size_t i = 0; i < knivesCount; ++i) {
    const float t = wrap01(vm.phase * 0.62f + static_cast<float>(i) * 0.18f);
    const int x = attackStartX - static_cast<int>(t * static_cast<float>(attackDistance));
    const int yOffset = (static_cast<int>(i) - static_cast<int>(knivesCount - 1) / 2) * laneSpacing;
    const int yJitter = attacking ? static_cast<int>(std::sin(vm.phase * 11.0f + static_cast<float>(i)) * 5.0f) : 0;
    const int y = centerLaneY + yOffset + yJitter;
    drawKnife(display_, x, y, "PR");

    if (attacking && i == knivesCount - 1 && t > 0.88f && t < 0.98f) {
      gotHit = true;
    }
  }

  if (gotHit) {
    drawCutMark(display_, pomberoX, pomberoY, vm.frame);
  }

  const uint16_t barColor = statusColor(vm.appState);
  display_.fillRect(layout_.statusBar.x, layout_.statusBar.y, layout_.statusBar.w, layout_.statusBar.h, barColor);
  display_.setTextColor(0xFFFF, barColor);
  display_.drawString(statusText(vm.appState), 6, layout_.statusBar.y + 6, 2);
}

}  // namespace dragonwatch::ui
