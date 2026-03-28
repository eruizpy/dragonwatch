#include "dragon_animator.h"

namespace dragonwatch::ui {

void DragonAnimator::draw(dragonwatch::platform::IDisplayAdapter& display,
                          dragonwatch::domain::DragonAnimationState state,
                          uint32_t frame,
                          int centerX,
                          int centerY) {
  const bool blink = (frame / 10) % 2 == 0;
  const bool scareWave = (frame / 6) % 2 == 0;
  const bool hasPr = state == dragonwatch::domain::DragonAnimationState::Working;

  const uint16_t skinColor = 0xFD20;
  const uint16_t clothColor = state == dragonwatch::domain::DragonAnimationState::Error ? 0xF800 : 0x39E7;
  const uint16_t eyeColor = blink ? 0xFFFF : 0x0000;

  // Head
  display.fillCircle(centerX, centerY - 34, 12, skinColor);
  display.fillCircle(centerX - 4, centerY - 36, 2, eyeColor);
  display.fillCircle(centerX + 4, centerY - 36, 2, eyeColor);

  // Torso
  display.fillRect(centerX - 13, centerY - 20, 26, 34, clothColor);
  display.fillRect(centerX - 6, centerY - 15, 12, 7, 0x0000);

  // Legs
  display.fillRect(centerX - 12, centerY + 14, 9, 20, 0x7BEF);
  display.fillRect(centerX + 3, centerY + 14, 9, 20, 0x7BEF);

  // Arms: with PR (Working), arms stay raised and shake up/down to "scare".
  if (hasPr) {
    const int yTop = scareWave ? centerY - 50 : centerY - 44;
    display.fillRect(centerX - 27, yTop, 8, 24, skinColor);
    display.fillRect(centerX + 19, yTop, 8, 24, skinColor);
  } else {
    const int yArm = centerY - 10;
    display.fillRect(centerX - 27, yArm, 8, 22, skinColor);
    display.fillRect(centerX + 19, yArm, 8, 22, skinColor);
  }
}

}  // namespace dragonwatch::ui
