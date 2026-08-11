#include "framework/constants.hpp"
#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/systems/player_movement.hpp"
#include <cstdint>
#include <gtest/gtest.h>

namespace testing::player_movement_system {

constexpr int PLAYER_WIDTH = 30;
constexpr uint8_t MOVE_SPEED = 6;

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;

    systems::PlayerMovement system;

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
      .system = systems::PlayerMovement(),
  };
}

entt::entity dummy_player(entt::registry &ecs, float x, float y, bool is_moving = false, bool left = false) {
  auto entity = ecs.create();

  ecs.emplace<components::Position>(
      entity, components::Position{.x = x, .y = y, .w = PLAYER_WIDTH, .h = 40, .z = 50}
  );

  ecs.emplace<components::PlayerMovement>(entity, components::PlayerMovement{.x_speed = MOVE_SPEED});

  ecs.emplace<components::Animation>(
      entity, components::Animation{.playing = is_moving, .play_reversed = left}
  );

  return entity;
}

void assert_dummy_player(
    entt::registry &ecs, entt::entity entity, float x, float y, bool is_moving = false, bool left = false
) {
  const auto expected_position = components::Position{.x = x, .y = y, .w = PLAYER_WIDTH, .h = 40, .z = 50};
  EXPECT_EQ(expected_position, ecs.get<components::Position>(entity));

  const auto expected_movement = components::PlayerMovement{.x_speed = MOVE_SPEED};
  EXPECT_EQ(expected_movement, ecs.get<components::PlayerMovement>(entity));

  const auto expected_animation = components::Animation{.playing = is_moving, .play_reversed = left};
  EXPECT_EQ(expected_animation, ecs.get<components::Animation>(entity));
}

TEST(SystemPlayerMovement, ExecuteLeftAndRightNotEngagedShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 10, 11);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 10, 11);
}

TEST(SystemPlayerMovement, ExecuteLeftAndRightBothEngagedShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 10, 11);
  setup.player_input.engage(framework::PlayerInput::LEFT);
  setup.player_input.engage(framework::PlayerInput::RIGHT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 10, 11);
}

TEST(SystemPlayerMovement, ExecuteLeftEngagedShouldRunLeftAnimationAndMoveLeft) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 10, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::LEFT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 10 - MOVE_SPEED, 11, true, true);
}

TEST(
    SystemPlayerMovement, ExecuteLeftEngagedAndWithinSpeedOfWallShouldRunLeftAnimationAndMoveLeftButStopAtWall
) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, MOVE_SPEED - 1, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::LEFT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 0, 11, true, true);
}

TEST(SystemPlayerMovement, ExecuteLeftEngagedButAlreadyAtWallShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 0, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::LEFT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 0, 11, false, false);
}

TEST(SystemPlayerMovement, ExecuteRightEngagedShouldRunRightAnimationAndMoveRight) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 10, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::RIGHT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 10 + MOVE_SPEED, 11, true, false);
}

TEST(
    SystemPlayerMovement,
    ExecuteRightEngagedAndWithinSpeedOfWallShouldRunRightAnimationAndMoveRightButStopAtWall
) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, framework::WINDOW_WIDTH - PLAYER_WIDTH - 1, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::RIGHT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, framework::WINDOW_WIDTH - PLAYER_WIDTH, 11, true, false);
}

TEST(SystemPlayerMovement, ExecuteRightEngagedButAlreadyAtWallShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, framework::WINDOW_WIDTH - PLAYER_WIDTH, 11, false, true);
  setup.player_input.engage(framework::PlayerInput::RIGHT);

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, framework::WINDOW_WIDTH - PLAYER_WIDTH, 11, false, true);
}

TEST(SystemPlayerMovement, ExecuteGameIsPausedShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = dummy_player(setup.ecs, 10, 11, false, false);
  setup.player_input.engage(framework::PlayerInput::LEFT);
  ctx.ecs.ctx().emplace<components::Pause>();

  setup.system.execute(ctx);

  assert_dummy_player(setup.ecs, entity, 10, 11, false, false);
}

} // namespace testing::player_movement_system