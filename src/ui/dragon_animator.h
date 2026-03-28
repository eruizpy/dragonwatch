#pragma once

#include "../domain/animation/dragon_state_machine.h"
#include "../platform/display_adapter.h"

namespace dragonwatch::ui {

class DragonAnimator {
 public:
  void draw(dragonwatch::platform::IDisplayAdapter& display,
            dragonwatch::domain::DragonAnimationState state,
            uint32_t frame,
            int centerX,
            int centerY);
};

}  // namespace dragonwatch::ui
