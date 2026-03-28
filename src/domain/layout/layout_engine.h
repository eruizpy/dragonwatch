#pragma once

namespace dragonwatch::domain {

struct Rect {
  int x;
  int y;
  int w;
  int h;
};

struct ScreenLayout {
  Rect header;
  Rect dragonArea;
  Rect commitArea;
  Rect statusBar;
};

ScreenLayout computeLayout(int width, int height);

}  // namespace dragonwatch::domain
