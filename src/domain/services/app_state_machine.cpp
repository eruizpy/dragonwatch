#include "app_state_machine.h"

namespace dragonwatch::domain {

AppState transitionAppState(AppState current, DataEvent event) {
  switch (event) {
    case DataEvent::Start:
      return AppState::LOADING;
    case DataEvent::WifiLost:
      return AppState::RECONNECTING;
    case DataEvent::WifiRecovered:
      return AppState::LOADING;
    case DataEvent::FetchSuccessWithData:
      return AppState::READY;
    case DataEvent::FetchSuccessNoData:
      return AppState::NO_PRS;
    case DataEvent::FetchNetworkError:
      return AppState::ERROR_NETWORK;
    case DataEvent::FetchApiError:
      return AppState::ERROR_API;
    case DataEvent::FetchRateLimit:
      return AppState::RATE_LIMIT;
    default:
      return current;
  }
}

}  // namespace dragonwatch::domain
