#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/systems/deletion.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>

namespace testing::deletion_system {

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    systems::Deletion system;

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
      .system = systems::Deletion(),
  };
}

TEST(SystemDeletion, ExecuteEntityIsTaggedToDeleteShouldBeDeleted) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::ToBeDeleted>(entity);

  EXPECT_TRUE(ctx.ecs.valid(entity));
  setup.system.execute(ctx);
  EXPECT_FALSE(ctx.ecs.valid(entity));
}

TEST(SystemDeletion, ExecuteEntityIsNotTaggedToDeleteShouldNotBeDeleted) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();

  EXPECT_TRUE(ctx.ecs.valid(entity));
  setup.system.execute(ctx);
  EXPECT_TRUE(ctx.ecs.valid(entity));
}

} // namespace testing::deletion_system