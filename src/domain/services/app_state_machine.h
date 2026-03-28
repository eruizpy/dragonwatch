#pragma once

#include "../models/app_state.h"

namespace dragonwatch::domain {

AppState transitionAppState(AppState current, DataEvent event);

}  // namespace dragonwatch::domain
