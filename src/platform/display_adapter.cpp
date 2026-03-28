#include "display_adapter.h"

#include <TFT_eSPI.h>

namespace dragonwatch::platform {

class TftEsPiDisplayAdapter::Impl {
 public:
  TFT_eSPI tft;
};

TftEsPiDisplayAdapter::TftEsPiDisplayAdapter() : impl_(new Impl()) {}
TftEsPiDisplayAdapter::~TftEsPiDisplayAdapter() { delete impl_; }

void TftEsPiDisplayAdapter::begin() { impl_->tft.init(); }
void TftEsPiDisplayAdapter::setRotation(uint8_t rotation) { impl_->tft.setRotation(rotation); }
void TftEsPiDisplayAdapter::fillScreen(uint16_t color) { impl_->tft.fillScreen(color); }
void TftEsPiDisplayAdapter::fillRect(int x, int y, int w, int h, uint16_t color) {
  impl_->tft.fillRect(x, y, w, h, color);
}
void TftEsPiDisplayAdapter::drawRect(int x, int y, int w, int h, uint16_t color) {
  impl_->tft.drawRect(x, y, w, h, color);
}
void TftEsPiDisplayAdapter::fillCircle(int x, int y, int radius, uint16_t color) {
  impl_->tft.fillCircle(x, y, radius, color);
}
void TftEsPiDisplayAdapter::drawString(const char* text, int x, int y, int font) {
  impl_->tft.drawString(text, x, y, font);
}
void TftEsPiDisplayAdapter::setTextColor(uint16_t fg, uint16_t bg) { impl_->tft.setTextColor(fg, bg); }

}  // namespace dragonwatch::platform
