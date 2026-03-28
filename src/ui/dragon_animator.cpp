#include "dragon_animator.h"

namespace dragonwatch::ui {

void DragonAnimator::draw(dragonwatch::platform::IDisplayAdapter& display,
                          dragonwatch::domain::DragonAnimationState state,
                          uint32_t frame,
                          int centerX,
                          int centerY) {
  const bool blink = (frame / 10) % 2 == 0;
  const bool running = state == dragonwatch::domain::DragonAnimationState::Working;
  const bool stepA = (frame / 4) % 2 == 0;

  const uint16_t skinColor = 0xFD20;
  const uint16_t clothColor = state == dragonwatch::domain::DragonAnimationState::Error ? 0xF800 : 0x39E7;
  const uint16_t eyeColor = blink ? 0xFFFF : 0x0000;

  // Head
  display.fillCircle(centerX, centerY - 32, 11, skinColor);
  display.fillRect(centerX - 5, centerY - 35, 3, 2, eyeColor);
  display.fillRect(centerX + 2, centerY - 35, 3, 2, eyeColor);

  // Torso
  display.fillRect(centerX - 12, centerY - 18, 24, 30, clothColor);
  display.fillRect(centerX - 6, centerY - 14, 12, 6, 0x0000);

  // Arms + legs in running cycle.
  if (running) {
    if (stepA) {
      display.fillRect(centerX - 24, centerY - 16, 8, 18, skinColor);
      display.fillRect(centerX + 16, centerY - 5, 8, 18, skinColor);
      display.fillRect(centerX - 12, centerY + 12, 9, 21, 0x7BEF);
      display.fillRect(centerX + 3, centerY + 17, 9, 16, 0x7BEF);
    } else {
      display.fillRect(centerX - 24, centerY - 5, 8, 18, skinColor);
      display.fillRect(centerX + 16, centerY - 16, 8, 18, skinColor);
      display.fillRect(centerX - 12, centerY + 17, 9, 16, 0x7BEF);
      display.fillRect(centerX + 3, centerY + 12, 9, 21, 0x7BEF);
    }
  } else {
    display.fillRect(centerX - 24, centerY - 8, 8, 18, skinColor);
    display.fillRect(centerX + 16, centerY - 8, 8, 18, skinColor);
    display.fillRect(centerX - 12, centerY + 14, 9, 19, 0x7BEF);
    display.fillRect(centerX + 3, centerY + 14, 9, 19, 0x7BEF);
  }

  (void)running;
}

}  // namespace dragonwatch::ui
