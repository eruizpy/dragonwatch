#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../domain/animation/dragon_state_machine.h"
#include "../domain/models/app_state.h"
#include "../domain/models/commit_info.h"

namespace dragonwatch::ui {

struct ScreenViewModel {
  dragonwatch::domain::AppState appState = dragonwatch::domain::AppState::LOADING;
  std::string repo;
  int prNumber = 0;
  std::string prTitle;
  std::string prAuthor;
  std::vector<dragonwatch::domain::CommitInfo> commits;
  dragonwatch::domain::DragonAnimationState dragonState = dragonwatch::domain::DragonAnimationState::Idle;
  uint32_t frame = 0;
  float phase = 0.0f;
};

}  // namespace dragonwatch::ui
