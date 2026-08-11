#include "framework/animation_strip_registry.hpp"
#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/animation_unpausable.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/systems/animation.hpp"
#include <entt.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace testing::animation_system {

struct TestSetup {
    entt::registry ecs;
    framework::EventBroker events;
    framework::PlayerInputManager player_input;
    framework::AnimationStripRegistry animation_strips;

    std::unique_ptr<systems::Animation> system;

    framework::ExecuteCtx ctx() {
      return framework::ExecuteCtx{
          .ecs = ecs,
          .events = events,
          .player_input = player_input,
      };
    }
};

components::Sprite default_dummy_sprite() {
  return components::Sprite{
      .src_id = "test",
      .src_x = 1.0F,
      .src_y = 2.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
}

TestSetup setupTest() {
  TestSetup result;

  result.system = std::make_unique<systems::Animation>(result.animation_strips);

  return result;
}

TEST(SystemAnimation, ExecuteAnimationIsNotPlayingShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 10});
  setup.animation_strips.add_frame(strip_id, {.x = 2, .y = 20});
  ctx.ecs.emplace<components::Sprite>(entity, default_dummy_sprite());
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 100,
          .ticks_per_frame = 1,
          .strip_id = strip_id,
          .playing = false,
      }
  );

  setup.system->execute(ctx);

  EXPECT_EQ(default_dummy_sprite(), ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 100,
      .ticks_per_frame = 1,
      .strip_id = strip_id,
      .playing = false,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteAnimationStripHasNoFramesShouldDoNothing) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  ctx.ecs.emplace<components::Sprite>(entity, default_dummy_sprite());
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 100,
          .ticks_per_frame = 1,
          .strip_id = strip_id,
          .playing = true,
      }
  );

  setup.system->execute(ctx);

  EXPECT_EQ(default_dummy_sprite(), ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 100,
      .ticks_per_frame = 1,
      .strip_id = strip_id,
      .playing = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteCounterHasNotYetReachedTicksPerFrameShouldIncrementCounter) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 10});
  setup.animation_strips.add_frame(strip_id, {.x = 2, .y = 20});
  ctx.ecs.emplace<components::Sprite>(entity, default_dummy_sprite());
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 0,
          .ticks_per_frame = 2,
          .strip_id = strip_id,
          .playing = true,
      }
  );

  setup.system->execute(ctx);

  EXPECT_EQ(default_dummy_sprite(), ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 1,
      .ticks_per_frame = 2,
      .strip_id = strip_id,
      .playing = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteCounterHasReachesTicksPerFrameShouldMoveToNextFrame) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 0,
          .strip_id = strip_id,
          .playing = true,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 9.0F,
      .src_y = 16.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 1,
      .strip_id = strip_id,
      .playing = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteShiftFrameWhenAlreadyAtLastOneShouldMoveToFirstFrame) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 2,
          .strip_id = strip_id,
          .playing = true,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 3.0F,
      .src_y = 8.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 0,
      .strip_id = strip_id,
      .playing = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteShiftFrameWhenPlayingBackwardsShouldDecrementFrame) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 1,
          .strip_id = strip_id,
          .playing = true,
          .play_reversed = true,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 3.0F,
      .src_y = 8.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 0,
      .strip_id = strip_id,
      .playing = true,
      .play_reversed = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteShiftFrameWhenPlayingBackwardsAndAlreadyAtFirstFrameShouldGoToLastFrame) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 0,
          .strip_id = strip_id,
          .playing = true,
          .play_reversed = true,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 15.0F,
      .src_y = 24.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 2,
      .strip_id = strip_id,
      .playing = true,
      .play_reversed = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteGameIsPausedShouldNotExecuteRegularAnimations) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::Animation>(
      entity,
      components::Animation{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 0,
          .strip_id = strip_id,
          .playing = true,
      }
  );
  ctx.ecs.ctx().emplace<components::Pause>();

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 1.0F,
      .src_y = 2.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::Animation{
      .tick_counter = 1,
      .ticks_per_frame = 2,
      .cur_frame = 0,
      .strip_id = strip_id,
      .playing = true,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::Animation>(entity));
}

TEST(SystemAnimation, ExecuteGameIsPausedShouldStillExecuteUnpausableAnimations) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::AnimationUnpausable>(
      entity,
      components::AnimationUnpausable{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 0,
          .strip_id = strip_id,
      }
  );
  ctx.ecs.ctx().emplace<components::Pause>();

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 9.0F,
      .src_y = 16.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::AnimationUnpausable{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 1,
      .strip_id = strip_id,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::AnimationUnpausable>(entity));
}

TEST(SystemAnimation, ExecuteGameIsNotPausedShouldStillExecuteUnpausableAnimations) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::AnimationUnpausable>(
      entity,
      components::AnimationUnpausable{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 0,
          .strip_id = strip_id,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 9.0F,
      .src_y = 16.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::AnimationUnpausable{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 1,
      .strip_id = strip_id,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::AnimationUnpausable>(entity));
}

TEST(SystemAnimation, ExecuteUnpausableAnimationHasReachedEndOfStripShouldWrapAroundToFirstFrame) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::AnimationUnpausable>(
      entity,
      components::AnimationUnpausable{
          .tick_counter = 1,
          .ticks_per_frame = 2,
          .cur_frame = 2,
          .strip_id = strip_id,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 3.0F,
      .src_y = 8.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::AnimationUnpausable{
      .tick_counter = 0,
      .ticks_per_frame = 2,
      .cur_frame = 0,
      .strip_id = strip_id,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::AnimationUnpausable>(entity));
}

TEST(SystemAnimation, ExecuteUnpausableAnimationNotYetAtFrameChangeShouldIncrTicks) {
  TestSetup setup = setupTest();
  auto ctx = setup.ctx();
  auto entity = ctx.ecs.create();
  auto strip_id = setup.animation_strips.create();
  setup.animation_strips.add_frame(strip_id, {.x = 1, .y = 2});
  setup.animation_strips.add_frame(strip_id, {.x = 3, .y = 4});
  setup.animation_strips.add_frame(strip_id, {.x = 5, .y = 6});
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "test",
          .src_x = 1.0F,
          .src_y = 2.0F,
          .src_w = 3.0F,
          .src_h = 4.0F,
          .dst_w = 5.0F,
          .dst_h = 6.0F,
      }
  );
  ctx.ecs.emplace<components::AnimationUnpausable>(
      entity,
      components::AnimationUnpausable{
          .tick_counter = 0,
          .ticks_per_frame = 2,
          .cur_frame = 2,
          .strip_id = strip_id,
      }
  );

  setup.system->execute(ctx);

  const auto expected_sprite = components::Sprite{
      .src_id = "test",
      .src_x = 1.0F,
      .src_y = 2.0F,
      .src_w = 3.0F,
      .src_h = 4.0F,
      .dst_w = 5.0F,
      .dst_h = 6.0F,
  };
  EXPECT_EQ(expected_sprite, ctx.ecs.get<components::Sprite>(entity));
  const auto expected_animation = components::AnimationUnpausable{
      .tick_counter = 1,
      .ticks_per_frame = 2,
      .cur_frame = 2,
      .strip_id = strip_id,
  };
  EXPECT_EQ(expected_animation, ctx.ecs.get<components::AnimationUnpausable>(entity));
}

} // namespace testing::animation_system