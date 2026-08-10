#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/damage.hpp"
#include "game/scenes/invasion/components/damage_callback.hpp"
#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/systems/damage.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>
#include <optional>

namespace testing::damage_system {

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

class SystemDamage : public ::testing::Test {
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

    systems::Damage system;

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
      .system = systems::Damage(*scripts, *callbacks),
  };
}

TEST_F(SystemDamage, ExecuteNoCollisionsOccurShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 1,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .cur_hitpoints = 1,
      }
  );

  setup.system.execute(ctx);

  EXPECT_EQ(1, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteCollisionsOccursButReceiverDoesntHaveHitpointsShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteCollisionsOccursButHitterDoesntDoDamageShouldNotAlterHitpoints) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Player),
          .cur_hitpoints = 10,
      }
  );
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_EQ(10, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteCollisionsOccursButDamageTypesDontMatchShouldNotAlterHitpoints) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Player),
          .cur_hitpoints = 10,
      }
  );
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_EQ(10, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteCollisionsOccursAndDamageTypeMatchesShouldDecreaseHitpointsByDamage) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .cur_hitpoints = 10,
      }
  );
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_EQ(4, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(
    SystemDamage, ExecuteCollisionsOccursAndDamageTypesAreDifferentButOverlapShouldDecreaseHitpointsByDamage
) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = components::DamageType::Alien | components::DamageType::Fortress,
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = components::DamageType::Alien | components::DamageType::Player_Projectile
                            | components::DamageType::Alien_Projectile,
          .cur_hitpoints = 10,
      }
  );
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_EQ(4, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_FALSE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteCollisionsOccursDamageDecreasesHitpointsBelowOneShouldAddDeletionFlag) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .cur_hitpoints = 6,
      }
  );
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});

  setup.system.execute(ctx);

  EXPECT_EQ(0, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

TEST_F(SystemDamage, ExecuteDamagedEntityHasCallbackShouldInvokeCallback) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto who_i_am = ctx.ecs.create();
  ctx.ecs.emplace<components::Damage>(
      who_i_am,
      components::Damage{
          .type = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .amount = 6,
      }
  );
  auto who_i_hit = ctx.ecs.create();
  ctx.ecs.emplace<components::Hitpoints>(
      who_i_hit,
      components::Hitpoints{
          .susceptible_to = static_cast<components::DamageTypeSet>(components::DamageType::Alien),
          .cur_hitpoints = 6,
      }
  );
  ctx.ecs.emplace<components::DamageCallback>(who_i_hit, components::DamageCallback{.callback = 1});
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = who_i_am, .who_i_hit = who_i_hit});
  scripts->call_function("pack", "resetCallCount");

  setup.system.execute(ctx);

  EXPECT_EQ(1, scripts->call_function<double>("pack", "getCallCount"));
  EXPECT_EQ(1, scripts->call_function<double>("pack", "assertCalledWith", entt::to_integral(who_i_hit)));
  EXPECT_EQ(0, ctx.ecs.get<components::Hitpoints>(who_i_hit).cur_hitpoints);
  EXPECT_TRUE(ctx.ecs.all_of<components::ToBeDeleted>(who_i_hit));
}

} // namespace testing::damage_system