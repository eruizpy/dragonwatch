#include "layout_engine.h"

namespace dragonwatch::domain {

ScreenLayout computeLayout(int width, int height) {
  const int headerH = 64;
  const int statusH = 26;
  const int contentH = height - headerH - statusH;

  ScreenLayout layout{};
  layout.header = {0, 0, width, headerH};
  layout.dragonArea = {0, headerH, static_cast<int>(width * 0.62f), contentH};
  layout.commitArea = {layout.dragonArea.w, headerH, width - layout.dragonArea.w, contentH};
  layout.statusBar = {0, height - statusH, width, statusH};
  return layout;
}

}  // namespace dragonwatch::domain
