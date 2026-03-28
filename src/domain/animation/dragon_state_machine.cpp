#include "dragon_state_machine.h"

namespace dragonwatch::domain {

DragonAnimationState resolveDragonState(AppState appState) {
  switch (appState) {
    case AppState::READY:
      return DragonAnimationState::Working;
    case AppState::ERROR_API:
    case AppState::ERROR_NETWORK:
    case AppState::RATE_LIMIT:
      return DragonAnimationState::Error;
    default:
      return DragonAnimationState::Idle;
  }
}

}  // namespace dragonwatch::domain
