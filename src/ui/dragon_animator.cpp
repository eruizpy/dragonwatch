#include "dragon_animator.h"

namespace dragonwatch::ui {

void DragonAnimator::draw(dragonwatch::platform::IDisplayAdapter& display,
                          dragonwatch::domain::DragonAnimationState state,
                          uint32_t frame,
                          int centerX,
                          int centerY) {
  const bool blink = (frame / 12) % 2 == 0;
  const uint16_t bodyColor = state == dragonwatch::domain::DragonAnimationState::Error
                                 ? 0xF800
                                 : (state == dragonwatch::domain::DragonAnimationState::Working ? 0x07E0 : 0xAFE5);

  display.fillCircle(centerX, centerY, 34, bodyColor);
  display.fillCircle(centerX - 26, centerY - 8, 16, bodyColor);
  display.fillCircle(centerX + 25, centerY + 3, 9, 0x03EF);
  display.fillCircle(centerX + 27, centerY + 1, 3, blink ? 0xFFFF : 0x0000);
}

}  // namespace dragonwatch::ui
