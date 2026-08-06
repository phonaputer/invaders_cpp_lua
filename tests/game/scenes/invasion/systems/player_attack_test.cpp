#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/player_attack.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/systems/player_attack.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>
#include <lua.h>
#include <memory>

namespace testing::player_attack_system {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<framework::ScriptEnvironment> scripts;

const std::string TEST_LUAU = R"(
  local Pack = {}

  Pack.CallCount = 0
  Pack.CallX = 0
  Pack.CallY = 0
  Pack.CallEntity = 0

  function Pack.callback(entity: number, x: number, y: number): ()
    Pack.CallCount += 1
    Pack.CallX = x
    Pack.CallY = y
    Pack.CallEntity = entity
  end

  function Pack.reset(): ()
    Pack.CallCount = 0
    Pack.CallX = 0
    Pack.CallY = 0
    Pack.CallEntity = 0
  end

  function Pack.getCallCount(): number
    return Pack.CallCount
  end

  function Pack.validateLastCallWas(entity: number, x: number, y: number): boolean
    if entity == Pack.CallEntity and x == Pack.CallX and y == Pack.CallY then
        return 1
    else
        return 0
    end
  end

  return Pack
)";

class SystemPlayerAttack : public ::testing::Test {
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
    systems::PlayerAttack system;

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
      .system = systems::PlayerAttack(*scripts),
  };
}

TEST_F(SystemPlayerAttack, ExecuteCounterHasReachedTicksAndPlayerIsFiringShouldInvokeCallback) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(entity, components::Position{.x = 1, .y = 2});
  ctx.ecs.emplace<components::PlayerAttack>(
      entity,
      components::PlayerAttack{
          .ticks_per_attack = 10,
          .tick_counter = 10,
          .callback_package = "pack",
          .callback_function = "callback",
      }
  );
  setup.player_input.engage(framework::PlayerInput::FIRE);
  scripts->call_function("pack", "reset");

  setup.system.execute(ctx);

  EXPECT_EQ(1, scripts->call_function<double>("pack", "getCallCount"));
  EXPECT_EQ(
      1,
      scripts->call_function<lua_Number>("pack", "validateLastCallWas", entt::to_integral(entity), 1.0F, 2.0F)
  );
  const auto expected_player_attack = components::PlayerAttack{
      .ticks_per_attack = 10,
      .tick_counter = 0,
      .callback_package = "pack",
      .callback_function = "callback",
  };
  EXPECT_EQ(expected_player_attack, setup.ecs.get<components::PlayerAttack>(entity));
}

TEST_F(SystemPlayerAttack, ExecuteCounterHasReachedTicksAndPlayerIsNotFiringShouldIncrementTickerAndNotFire) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(entity, components::Position{.x = 1, .y = 2});
  ctx.ecs.emplace<components::PlayerAttack>(
      entity,
      components::PlayerAttack{
          .ticks_per_attack = 10,
          .tick_counter = 10,
          .callback_package = "pack",
          .callback_function = "callback",
      }
  );
  scripts->call_function("pack", "reset");

  setup.system.execute(ctx);

  EXPECT_EQ(0, scripts->call_function<double>("pack", "getCallCount"));
  const auto expected_player_attack = components::PlayerAttack{
      .ticks_per_attack = 10,
      .tick_counter = 11,
      .callback_package = "pack",
      .callback_function = "callback",
  };
  EXPECT_EQ(expected_player_attack, setup.ecs.get<components::PlayerAttack>(entity));
}

TEST_F(SystemPlayerAttack, ExecuteCounterHasNotReachedTicksAndPlayerIsFiringShouldIncrementTickerAndNotFire) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(entity, components::Position{.x = 1, .y = 2});
  ctx.ecs.emplace<components::PlayerAttack>(
      entity,
      components::PlayerAttack{
          .ticks_per_attack = 10,
          .tick_counter = 9,
          .callback_package = "pack",
          .callback_function = "callback",
      }
  );
  setup.player_input.engage(framework::PlayerInput::FIRE);
  scripts->call_function("pack", "reset");

  setup.system.execute(ctx);

  EXPECT_EQ(0, scripts->call_function<double>("pack", "getCallCount"));
  const auto expected_player_attack = components::PlayerAttack{
      .ticks_per_attack = 10,
      .tick_counter = 10,
      .callback_package = "pack",
      .callback_function = "callback",
  };
  EXPECT_EQ(expected_player_attack, setup.ecs.get<components::PlayerAttack>(entity));
}

} // namespace testing::player_attack_system