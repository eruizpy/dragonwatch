#include "status_badge.h"

namespace dragonwatch::ui::widgets {

const char* labelForState(dragonwatch::domain::AppState state) {
  switch (state) {
    case dragonwatch::domain::AppState::LOADING:
      return "Loading";
    case dragonwatch::domain::AppState::READY:
      return "Ready";
    case dragonwatch::domain::AppState::NO_PRS:
      return "No PRs";
    case dragonwatch::domain::AppState::RECONNECTING:
      return "Reconnecting";
    case dragonwatch::domain::AppState::ERROR_NETWORK:
      return "Network error";
    case dragonwatch::domain::AppState::ERROR_API:
      return "API error";
    case dragonwatch::domain::AppState::RATE_LIMIT:
      return "Rate limited";
    default:
      return "Unknown";
  }
}

}  // namespace dragonwatch::ui::widgets
