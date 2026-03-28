#pragma once

#include <cstdint>

namespace dragonwatch::platform {

class IDisplayAdapter {
 public:
  virtual ~IDisplayAdapter() = default;
  virtual void begin() = 0;
  virtual void setRotation(uint8_t rotation) = 0;
  virtual void fillScreen(uint16_t color) = 0;
  virtual void fillRect(int x, int y, int w, int h, uint16_t color) = 0;
  virtual void drawRect(int x, int y, int w, int h, uint16_t color) = 0;
  virtual void fillCircle(int x, int y, int radius, uint16_t color) = 0;
  virtual void drawString(const char* text, int x, int y, int font) = 0;
  virtual void setTextColor(uint16_t fg, uint16_t bg) = 0;
};

class TftEsPiDisplayAdapter final : public IDisplayAdapter {
 public:
  TftEsPiDisplayAdapter();
  ~TftEsPiDisplayAdapter() override;

  void begin() override;
  void setRotation(uint8_t rotation) override;
  void fillScreen(uint16_t color) override;
  void fillRect(int x, int y, int w, int h, uint16_t color) override;
  void drawRect(int x, int y, int w, int h, uint16_t color) override;
  void fillCircle(int x, int y, int radius, uint16_t color) override;
  void drawString(const char* text, int x, int y, int font) override;
  void setTextColor(uint16_t fg, uint16_t bg) override;

 private:
  class Impl;
  Impl* impl_;
};

}  // namespace dragonwatch::platform
