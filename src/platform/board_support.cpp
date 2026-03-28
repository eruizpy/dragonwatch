#include "board_support.h"

#include <Arduino.h>

#include "board_config.h"

namespace dragonwatch::platform {

void initBoard() {
  pinMode(DW_PIN_TFT_BL, OUTPUT);
  digitalWrite(DW_PIN_TFT_BL, HIGH);
}

}  // namespace dragonwatch::platform
