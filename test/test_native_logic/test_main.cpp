#include <cmath>
#include <string>
#include <vector>

#include <unity.h>

#include "../../src/app/scheduler.h"
#include "../../src/domain/animation/dragon_state_machine.h"
#include "../../src/domain/animation/floating_commits.h"
#include "../../src/domain/layout/layout_engine.h"
#include "../../src/domain/models/app_state.h"
#include "../../src/domain/models/pull_request.h"
#include "../../src/domain/services/app_state_machine.h"
#include "../../src/domain/services/backoff_policy.h"
#include "../../src/domain/services/github_json_parser.h"
#include "../../src/domain/services/pr_selector.h"
#include "../../src/domain/services/sha_utils.h"

// Compile domain/application pure implementations directly for native unit tests.
#include "../../src/app/scheduler.cpp"
#include "../../src/domain/animation/dragon_state_machine.cpp"
#include "../../src/domain/animation/floating_commits.cpp"
#include "../../src/domain/layout/layout_engine.cpp"
#include "../../src/domain/services/app_state_machine.cpp"
#include "../../src/domain/services/backoff_policy.cpp"
#include "../../src/domain/services/github_json_parser.cpp"
#include "../../src/domain/services/pr_selector.cpp"
#include "../../src/domain/services/sha_utils.cpp"

using namespace dragonwatch;
using namespace dragonwatch::domain;

void test_normalize_sha_truncates_long_value() {
  TEST_ASSERT_EQUAL_STRING("0123456", normalizeSha("0123456789abcdef").c_str());
}

void test_normalize_sha_keeps_short_value() {
  TEST_ASSERT_EQUAL_STRING("abc", normalizeSha("abc").c_str());
}

void test_select_initial_pr_index_handles_empty() {
  std::vector<PullRequest> prs;
  TEST_ASSERT_FALSE(selectInitialPrIndex(prs).has_value());
}

void test_select_next_pr_index_wraps() {
  std::vector<PullRequest> prs(3);
  auto next = selectNextPrIndex(prs, 2);
  TEST_ASSERT_TRUE(next.has_value());
  TEST_ASSERT_EQUAL_UINT32(0, next.value());
}

void test_scheduler_triggers_by_interval() {
  Scheduler scheduler({1000, 500, 16});
  TEST_ASSERT_TRUE(scheduler.shouldRefresh(0));
  TEST_ASSERT_FALSE(scheduler.shouldRefresh(500));
  TEST_ASSERT_TRUE(scheduler.shouldRefresh(1000));

  TEST_ASSERT_TRUE(scheduler.shouldRotate(0));
  TEST_ASSERT_FALSE(scheduler.shouldRotate(300));
  TEST_ASSERT_TRUE(scheduler.shouldRotate(500));
}

void test_backoff_doubles_and_resets() {
  BackoffPolicy backoff(1000, 8000);
  TEST_ASSERT_EQUAL_UINT32(1000, backoff.currentDelay());
  backoff.onFailure();
  TEST_ASSERT_EQUAL_UINT32(2000, backoff.currentDelay());
  backoff.onFailure();
  backoff.onFailure();
  TEST_ASSERT_EQUAL_UINT32(8000, backoff.currentDelay());
  backoff.onSuccess();
  TEST_ASSERT_EQUAL_UINT32(1000, backoff.currentDelay());
}

void test_app_state_transitions() {
  TEST_ASSERT_EQUAL(static_cast<int>(AppState::RECONNECTING),
                    static_cast<int>(transitionAppState(AppState::READY, DataEvent::WifiLost)));
  TEST_ASSERT_EQUAL(static_cast<int>(AppState::READY),
                    static_cast<int>(transitionAppState(AppState::LOADING, DataEvent::FetchSuccessWithData)));
  TEST_ASSERT_EQUAL(static_cast<int>(AppState::RATE_LIMIT),
                    static_cast<int>(transitionAppState(AppState::READY, DataEvent::FetchRateLimit)));
}

void test_dragon_state_resolution() {
  TEST_ASSERT_EQUAL(static_cast<int>(DragonAnimationState::Working),
                    static_cast<int>(resolveDragonState(AppState::READY)));
  TEST_ASSERT_EQUAL(static_cast<int>(DragonAnimationState::Error),
                    static_cast<int>(resolveDragonState(AppState::ERROR_NETWORK)));
  TEST_ASSERT_EQUAL(static_cast<int>(DragonAnimationState::Idle),
                    static_cast<int>(resolveDragonState(AppState::LOADING)));
}

void test_floating_positions_count_and_radius() {
  const auto pts = computeFloatingCommitPositions(4, 100.0f, 120.0f, 30.0f, 0.0f);
  TEST_ASSERT_EQUAL_UINT32(4, pts.size());

  const float dx = pts[0].x - 100.0f;
  const float dy = (pts[0].y - 120.0f) / 0.6f;
  const float r = std::sqrt(dx * dx + dy * dy);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 30.0f, r);
}

void test_layout_partitions_screen() {
  const auto layout = computeLayout(320, 240);
  TEST_ASSERT_EQUAL_INT(320, layout.header.w);
  TEST_ASSERT_EQUAL_INT(240, layout.statusBar.y + layout.statusBar.h);
  TEST_ASSERT_EQUAL_INT(320, layout.dragonArea.w + layout.commitArea.w);
}

void test_parse_pull_requests_maps_fields() {
  const std::string payload = R"([
    {"number": 12, "title": "Fix sensor", "user": {"login": "alice"}},
    {"number": 13, "title": "Tune UI", "user": {"login": "bob"}}
  ])";

  const auto prs = parsePullRequests("owner/repo", payload);
  TEST_ASSERT_EQUAL_UINT32(2, prs.size());
  TEST_ASSERT_EQUAL_INT(12, prs[0].number);
  TEST_ASSERT_EQUAL_STRING("owner/repo", prs[0].repo.c_str());
  TEST_ASSERT_EQUAL_STRING("alice", prs[0].author.c_str());
}

void test_parse_commits_maps_short_sha() {
  const std::string payload = R"([
    {"sha": "abcdef1234567890"},
    {"sha": "1111111222222333333"}
  ])";

  const auto commits = parseCommits(payload);
  TEST_ASSERT_EQUAL_UINT32(2, commits.size());
  TEST_ASSERT_EQUAL_STRING("abcdef1", commits[0].shortSha.c_str());
}

void test_parse_returns_empty_for_malformed_json() {
  const auto prs = parsePullRequests("owner/repo", "{not-json");
  TEST_ASSERT_TRUE(prs.empty());

  const auto commits = parseCommits("oops");
  TEST_ASSERT_TRUE(commits.empty());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_normalize_sha_truncates_long_value);
  RUN_TEST(test_normalize_sha_keeps_short_value);
  RUN_TEST(test_select_initial_pr_index_handles_empty);
  RUN_TEST(test_select_next_pr_index_wraps);
  RUN_TEST(test_scheduler_triggers_by_interval);
  RUN_TEST(test_backoff_doubles_and_resets);
  RUN_TEST(test_app_state_transitions);
  RUN_TEST(test_dragon_state_resolution);
  RUN_TEST(test_floating_positions_count_and_radius);
  RUN_TEST(test_layout_partitions_screen);
  RUN_TEST(test_parse_pull_requests_maps_fields);
  RUN_TEST(test_parse_commits_maps_short_sha);
  RUN_TEST(test_parse_returns_empty_for_malformed_json);

  return UNITY_END();
}
