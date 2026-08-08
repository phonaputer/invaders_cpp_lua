#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/collision_active.hpp"
#include "game/scenes/invasion/components/collision_passive.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"
#include "game/scenes/invasion/systems/collision_detection.hpp"
#include <entt.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace events {
void PrintTo(const CollisionOccurred &col, std::ostream *os) {
  *os << "CollisionOccurred(" << entt::to_integral(col.who_am_i) << "," << entt::to_integral(col.who_i_hit)
      << ")";
}
} // namespace events

namespace testing::collision_detection_system {

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    systems::CollisionDetection system;

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
      .system = systems::CollisionDetection(),
  };
}

entt::entity
createEntityWithHitbox(framework::ExecuteCtx &ctx, float x, float y, float w, float h, bool passive = false) {
  auto e = ctx.ecs.create();

  ctx.ecs.emplace<components::Position>(
      e,
      components::Position{
          .x = x,
          .y = y,
          .w = 1,
          .h = 1,
          .z = 100,
      }
  );
  if (passive) {
    ctx.ecs.emplace<components::CollisionPassive>(
        e,
        components::CollisionPassive{
            .hitbox_offset_x = 0,
            .hitbox_offset_y = 0,
            .hitbox_w = w,
            .hitbox_h = h,
        }
    );
  } else {
    ctx.ecs.emplace<components::CollisionActive>(
        e,
        components::CollisionActive{
            .hitbox_offset_x = 0,
            .hitbox_offset_y = 0,
            .hitbox_w = w,
            .hitbox_h = h,
        }
    );
  }

  return e;
}

void assertTotalHits(framework::ExecuteCtx &ctx, size_t expected) {
  auto num_hits = ctx.events.get_all<events::CollisionOccurred>().size();

  EXPECT_EQ(expected, num_hits / 2);
}

void assertHitEachOther(framework::ExecuteCtx &ctx, entt::entity left, entt::entity right) {
  auto hit_messages = ctx.events.get_all<events::CollisionOccurred>();

  EXPECT_THAT(
      hit_messages, testing::Contains(events::CollisionOccurred{.who_am_i = left, .who_i_hit = right})
  );
  EXPECT_THAT(
      hit_messages, testing::Contains(events::CollisionOccurred{.who_am_i = right, .who_i_hit = left})
  );
}

void assertHitNothing(framework::ExecuteCtx &ctx, entt::entity entity) {
  auto hit_messages = ctx.events.get_all<events::CollisionOccurred>();

  for (const auto &message : hit_messages) {
    EXPECT_FALSE(message.who_am_i == entity);
    EXPECT_FALSE(message.who_i_hit == entity);
  }
}

TEST(SystemCollisionDetection, ExecuteEntitiesExactlyOnTopOfEachOtherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 1, 1);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenOneEntityWithinAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 0, 0, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenOneEntityThroughBottomOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 1, 3);
  auto entity_two = createEntityWithHitbox(ctx, 2, 2, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenOneEntityThroughTopOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 3, 1, 2);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenOneEntityThroughLeftOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 0, 2, 2, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenOneEntityThroughRightOfAnotherShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 3, 2, 2, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenEntitiesBarelyTouchingShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 0, 0, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(
    SystemCollisionDetection, ExecuteBigEntityCrossingMultipleBucketsShouldStillRegisterCollisionsCorrectly
) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 8, 8, 16, 16);
  auto entity_two = createEntityWithHitbox(ctx, 8, 8, 1, 1);
  auto entity_three = createEntityWithHitbox(ctx, 23, 23, 1, 1);
  auto entity_four = createEntityWithHitbox(ctx, 8, 23, 1, 1);
  auto entity_five = createEntityWithHitbox(ctx, 23, 8, 1, 1);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 4);
  assertHitEachOther(ctx, entity_one, entity_two);
  assertHitEachOther(ctx, entity_one, entity_three);
  assertHitEachOther(ctx, entity_one, entity_four);
  assertHitEachOther(ctx, entity_one, entity_five);
}

TEST(SystemCollisionDetection, ExecuteEntitiesOverlapAtNegativeCoordinatesShouldMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, -10, -10, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, -10, -10, 1, 1);

  setup.system.execute(ctx);

  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteEntitiesTouchingInMultipleBucketsShouldOnlyRegisterOneHit) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 8, 8, 16, 16);
  auto entity_two = createEntityWithHitbox(ctx, 8, 8, 16, 16);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 1);
  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenEntityAboveOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 2, 5, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitNothing(ctx, entity_one);
  assertHitNothing(ctx, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenEntityBelowOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 2, -1, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitNothing(ctx, entity_one);
  assertHitNothing(ctx, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenEntityRightOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 5, 2, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitNothing(ctx, entity_one);
  assertHitNothing(ctx, entity_two);
}

TEST(SystemCollisionDetection, ExecuteWhenEntityLeftOfOtherShouldNOTMarkThemAsHavingCollided) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, -1, 2, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 3, 3);

  setup.system.execute(ctx);

  assertHitNothing(ctx, entity_one);
  assertHitNothing(ctx, entity_two);
}

TEST(SystemCollisionDetection, ExecuteMultipleEntitiesTouchingOneEntityShouldMarkAllOfThemAsHavingHitIt) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 3, 3);
  auto entity_two = createEntityWithHitbox(ctx, 3, 3, 1, 1);
  auto entity_three = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_four = createEntityWithHitbox(ctx, 1, 3, 1, 1);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 3);
  assertHitEachOther(ctx, entity_one, entity_two);
  assertHitEachOther(ctx, entity_one, entity_three);
  assertHitEachOther(ctx, entity_one, entity_four);
}

TEST(SystemCollisionDetection, ExecuteMultipleHitsInOneFrameShouldMarkAllOfThem) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_three = createEntityWithHitbox(ctx, 50, 50, 1, 1);
  auto entity_four = createEntityWithHitbox(ctx, 100, 100, 1, 1);
  auto entity_five = createEntityWithHitbox(ctx, 50, 50, 1, 1);
  auto entity_six = createEntityWithHitbox(ctx, 3, 3, 3, 3);
  auto entity_seven = createEntityWithHitbox(ctx, 4, 4, 1, 1);
  auto entity_eight = createEntityWithHitbox(ctx, 4, 4, 1, 1);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 5);
  assertHitEachOther(ctx, entity_one, entity_two);
  assertHitEachOther(ctx, entity_three, entity_five);
  assertHitNothing(ctx, entity_four);
  assertHitEachOther(ctx, entity_six, entity_seven);
  assertHitEachOther(ctx, entity_six, entity_eight);
  assertHitEachOther(ctx, entity_seven, entity_eight);
}

TEST(SystemCollisionDetection, ExecuteHitboxIntersectsRightSideOfLongEnitityShouldRegisterHit) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 100, 100, 15, 5, true);
  auto entity_two = createEntityWithHitbox(ctx, 114, 100, 1, 6);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 1);
  assertHitEachOther(ctx, entity_one, entity_two);
}

TEST(
    SystemCollisionDetection,
    ExecuteMultipleOverlapsAndAllButOneEntityArePassiveShouldOnlyCheckForTheActiveEntity
) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity_one = createEntityWithHitbox(ctx, 1, 1, 1, 1);
  auto entity_two = createEntityWithHitbox(ctx, 1, 1, 1, 1, true);
  auto entity_three = createEntityWithHitbox(ctx, 100, 100, 1, 1, true);
  auto entity_four = createEntityWithHitbox(ctx, 100, 100, 1, 1, true);
  auto entity_five = createEntityWithHitbox(ctx, 200, 200, 1, 1, true);
  auto entity_six = createEntityWithHitbox(ctx, 200, 200, 1, 1, true);

  setup.system.execute(ctx);

  assertTotalHits(ctx, 1);
  assertHitEachOther(ctx, entity_one, entity_two);
  assertHitNothing(ctx, entity_three);
  assertHitNothing(ctx, entity_four);
  assertHitNothing(ctx, entity_five);
  assertHitNothing(ctx, entity_six);
}

} // namespace testing::collision_detection_system