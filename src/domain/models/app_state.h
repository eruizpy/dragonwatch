#pragma once

namespace dragonwatch::domain {

enum class AppState {
  LOADING,
  READY,
  NO_PRS,
  RECONNECTING,
  ERROR_NETWORK,
  ERROR_API,
  RATE_LIMIT,
};

enum class DataEvent {
  Start,
  WifiLost,
  WifiRecovered,
  FetchSuccessWithData,
  FetchSuccessNoData,
  FetchNetworkError,
  FetchApiError,
  FetchRateLimit,
};

}  // namespace dragonwatch::domain
