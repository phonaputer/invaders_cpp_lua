#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/deletion_callback.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/ttl.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/systems/deletion.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <optional>

namespace testing::deletion_system {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<framework::ScriptEnvironment> scripts;

const std::string TEST_LUAU = R"(
  local Pack = {}

  Pack.CallCount = 0
  Pack.CalledEntity = -1

  function Pack.callback(entity: number): ()
    Pack.CallCount += 1
    Pack.CalledEntity = entity
  end

  function Pack.resetCallCount(): ()
    Pack.CallCount = 0
  end

  function Pack.getCallCount(): number
    return Pack.CallCount
  end

  function Pack.assertCalledWith(entity: number): number
    if entity == Pack.CalledEntity then
      return 1
    else 
      return 0
    end
  end

  return Pack
)";

class MockCallbackGetter : public infra::CallbackGetter {
  public:
    [[nodiscard]] std::optional<infra::Callback> get_callback(infra::CallbackID id) const override {
      if (id == 1) {
        return infra::Callback{.package = "pack", .function = "callback"};
      }
      return infra::Callback{.package = "", .function = ""};
    }
};

class SystemDeletion : public ::testing::Test {
  protected:
    static void SetUpTestSuite() {
      scripts = std::make_unique<framework::ScriptEnvironment>();
      scripts->exec_script_string(TEST_LUAU, "pack");
    }
};

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    std::shared_ptr<infra::CallbackGetter> callbacks;

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
  const std::shared_ptr<infra::CallbackGetter> callbacks = std::make_shared<MockCallbackGetter>();

  return TestSetup{
      .ecs = entt::registry(),
      .events = framework::EventBroker(),
      .player_input = framework::PlayerInputManager(),
      .callbacks = callbacks,
      .system = systems::Deletion(*scripts, *callbacks),
  };
}

TEST_F(SystemDeletion, ExecuteHasDeletionCallbackShouldCallTheCallbackAndDeleteTheEntity) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::ToBeDeleted>(entity);
  ctx.ecs.emplace<components::DeletionCallback>(entity, components::DeletionCallback{.callback = 1});
  scripts->call_function("pack", "resetCallCount");

  setup.system.execute(ctx);

  EXPECT_EQ(1, scripts->call_function<double>("pack", "getCallCount"));
  EXPECT_EQ(1, scripts->call_function<double>("pack", "assertCalledWith", entt::to_integral(entity)));
  EXPECT_FALSE(ctx.ecs.valid(entity));
}

TEST_F(SystemDeletion, ExecuteTTLHasNotReachedLifetimeShouldIncrement) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::TTL>(entity, components::TTL{.ticks_to_live = 100, .tick_counter = 99});

  setup.system.execute(ctx);

  EXPECT_TRUE(ctx.ecs.valid(entity));
  const auto expected_ttl = components::TTL{.ticks_to_live = 100, .tick_counter = 100};
  EXPECT_EQ(expected_ttl, setup.ecs.get<components::TTL>(entity));
}

TEST_F(SystemDeletion, ExecuteTTLReachesLifetimeShouldDeleteEntity) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::TTL>(entity, components::TTL{.ticks_to_live = 100, .tick_counter = 100});

  setup.system.execute(ctx);

  EXPECT_FALSE(ctx.ecs.valid(entity));
}

TEST_F(SystemDeletion, ExecuteEntityIsTaggedToDeleteShouldBeDeleted) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::ToBeDeleted>(entity);

  EXPECT_TRUE(ctx.ecs.valid(entity));
  setup.system.execute(ctx);
  EXPECT_FALSE(ctx.ecs.valid(entity));
}

TEST_F(SystemDeletion, ExecuteEntityIsNotTaggedToDeleteShouldNotBeDeleted) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();

  EXPECT_TRUE(ctx.ecs.valid(entity));
  setup.system.execute(ctx);
  EXPECT_TRUE(ctx.ecs.valid(entity));
}

} // namespace testing::deletion_system