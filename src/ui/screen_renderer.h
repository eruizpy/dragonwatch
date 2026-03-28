#pragma once

#include "../domain/layout/layout_engine.h"
#include "../platform/display_adapter.h"
#include "dragon_animator.h"
#include "view_model.h"

namespace dragonwatch::ui {

class ScreenRenderer {
 public:
  explicit ScreenRenderer(dragonwatch::platform::IDisplayAdapter& display);

  void begin(int width, int height);
  void render(const ScreenViewModel& vm);

 private:
  const char* statusText(dragonwatch::domain::AppState state) const;
  uint16_t statusColor(dragonwatch::domain::AppState state) const;

  dragonwatch::platform::IDisplayAdapter& display_;
  dragonwatch::domain::ScreenLayout layout_{};
  DragonAnimator animator_;
  bool initialized_ = false;
};

}  // namespace dragonwatch::ui
