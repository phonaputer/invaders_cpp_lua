#include "framework/constants.hpp"
#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/velocity.hpp"
#include "game/scenes/invasion/systems/velocity.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>

namespace testing::velocity_system {

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    systems::Velocity system;

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
      .system = systems::Velocity(),
  };
}

TEST(SystemVelocity, ExecuteEntityHasXAndYVelocityShouldMoveByThatAmount) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity, components::Position{.x = 11, .y = 12, .w = 13, .h = 14, .z = 100}
  );
  ctx.ecs.emplace<components::Velocity>(entity, components::Velocity{.x = 5, .y = 10});

  setup.system.execute(ctx);

  auto result = ctx.ecs.get<components::Position>(entity);
  const auto expected = components::Position{.x = 16, .y = 22, .w = 13, .h = 14, .z = 100};
  EXPECT_EQ(expected, result);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(entity));
}

TEST(SystemVelocity, ExecuteEntityIsOffscreenToTheLeftAfterMovingShouldBeMarkedForDeletion) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity, components::Position{.x = 0, .y = 12, .w = 5, .h = 14, .z = 100}
  );
  ctx.ecs.emplace<components::Velocity>(entity, components::Velocity{.x = -11, .y = 10});

  setup.system.execute(ctx);

  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(entity));
}

TEST(SystemVelocity, ExecuteEntityIsOffscreenToTheRightAfterMovingShouldBeMarkedForDeletion) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity, components::Position{.x = framework::WINDOW_WIDTH, .y = 12, .w = 13, .h = 14, .z = 100}
  );
  ctx.ecs.emplace<components::Velocity>(entity, components::Velocity{.x = 6, .y = 10});

  setup.system.execute(ctx);

  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(entity));
}

TEST(SystemVelocity, ExecuteEntityIsOffscreenToTheTopAfterMovingShouldBeMarkedForDeletion) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity, components::Position{.x = 10, .y = 0, .w = 13, .h = 5, .z = 100}
  );
  ctx.ecs.emplace<components::Velocity>(entity, components::Velocity{.x = 6, .y = -11});

  setup.system.execute(ctx);

  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(entity));
}

TEST(SystemVelocity, ExecuteEntityIsOffscreenToTheBottomAfterMovingShouldBeMarkedForDeletion) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity, components::Position{.x = 10, .y = framework::WINDOW_HEIGHT, .w = 13, .h = 14, .z = 100}
  );
  ctx.ecs.emplace<components::Velocity>(entity, components::Velocity{.x = 2, .y = 6});

  setup.system.execute(ctx);

  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(entity));
}

} // namespace testing::velocity_system