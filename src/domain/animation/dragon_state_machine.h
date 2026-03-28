#pragma once

#include "../models/app_state.h"

namespace dragonwatch::domain {

enum class DragonAnimationState { Idle, Working, Error };

DragonAnimationState resolveDragonState(AppState appState);

}  // namespace dragonwatch::domain
