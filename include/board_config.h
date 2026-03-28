#pragma once

#define DW_SCREEN_WIDTH 320
#define DW_SCREEN_HEIGHT 240
#define DW_SCREEN_ROTATION 1

#define DW_PIN_TFT_MISO 12
#define DW_PIN_TFT_MOSI 13
#define DW_PIN_TFT_SCLK 14
#define DW_PIN_TFT_CS 15
#define DW_PIN_TFT_DC 2
#define DW_PIN_TFT_RST -1
#define DW_PIN_TFT_BL 21

namespace dragonwatch {

struct BoardConfig {
  static constexpr int screenWidth = DW_SCREEN_WIDTH;
  static constexpr int screenHeight = DW_SCREEN_HEIGHT;
  static constexpr int rotation = DW_SCREEN_ROTATION;
};

}  // namespace dragonwatch
