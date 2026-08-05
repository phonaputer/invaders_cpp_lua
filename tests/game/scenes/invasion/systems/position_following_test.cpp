#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/position_following.hpp"
#include "game/scenes/invasion/systems/position_following.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>

namespace testing::position_following_system {

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    systems::PositionFollowing system;

    framework::ExecuteCtx ctx() {
      return framework::ExecuteCtx{
          .ecs = ecs,
          .events = events,
          .player_input = player_input,
      };
    }
};

TestSetup setupTest() {
  return TestSetup{
      .ecs = entt::registry(),
      .events = framework::EventBroker(),
      .player_input = framework::PlayerInputManager(),
      .system = systems::PositionFollowing(),
  };
}

TEST(SystemPositionFollowing, ExecuteLeaderPositionDifferentFromFollowerShouldMoveFollowerToLeaderPosition) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto leader = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      leader, components::Position{.x = 15, .y = 25, .w = 35, .h = 45, .z = 55}
  );

  auto follower = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      follower, components::Position{.x = 10, .y = 20, .w = 30, .h = 40, .z = 100}
  );
  ctx.ecs.emplace<components::PositionFollowing>(
      follower,
      components::PositionFollowing{.leader = entt::to_integral(leader), .x_offset = 1, .y_offset = -1}
  );

  setup.system.execute(ctx);

  auto result = ctx.ecs.get<components::Position>(follower);
  const auto expected = components::Position{.x = 16, .y = 24, .w = 30, .h = 40, .z = 100};
  EXPECT_EQ(expected, result);
}

TEST(SystemPositionFollowing, ExecuteLeaderDoesNotHavePositionShouldRemoveFollowingFromFollower) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto leader = ctx.ecs.create();
  auto follower = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      follower, components::Position{.x = 10, .y = 20, .w = 30, .h = 40, .z = 100}
  );
  ctx.ecs.emplace<components::PositionFollowing>(
      follower,
      components::PositionFollowing{.leader = entt::to_integral(leader), .x_offset = 1, .y_offset = -1}
  );

  setup.system.execute(ctx);

  EXPECT_FALSE(ctx.ecs.all_of<components::PositionFollowing>(follower));
}

TEST(SystemPositionFollowing, ExecuteLeaderWasDeletedShouldRemoveFollowingFromFollower) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto leader = ctx.ecs.create();
  auto follower = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      follower, components::Position{.x = 10, .y = 20, .w = 30, .h = 40, .z = 100}
  );
  ctx.ecs.emplace<components::PositionFollowing>(
      follower,
      components::PositionFollowing{.leader = entt::to_integral(leader), .x_offset = 1, .y_offset = -1}
  );

  ctx.ecs.destroy(leader);
  setup.system.execute(ctx);

  EXPECT_FALSE(ctx.ecs.all_of<components::PositionFollowing>(follower));
}

} // namespace testing::position_following_system