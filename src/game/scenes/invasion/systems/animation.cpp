#include "game/scenes/invasion/systems/animation.hpp"
#include "framework/animation_strip_registry.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/animation_unpausable.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/sprite.hpp"

namespace systems {

Animation::Animation(const framework::AnimationStripRegistry &animation_strips)
    : animation_strips{animation_strips} {
}

void Animation::execute(framework::ExecuteCtx &ctx) {
  execute_unpausable_animations(ctx);

  if (ctx.ecs.ctx().contains<components::Pause>()) {
    return;
  }

  execute_regular_animations(ctx);
}

void Animation::execute_unpausable_animations(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::AnimationUnpausable, components::Sprite>();

  for (auto [entity, animation, sprite] : view.each()) {
    animation.tick_counter++;
    if (animation.tick_counter < animation.ticks_per_frame) {
      continue;
    }

    animation.tick_counter = 0;

    auto strip = animation_strips.get().get(animation.strip_id);
    if (strip.empty()) {
      continue;
    }

    animation.cur_frame++;
    if (animation.cur_frame >= strip.size()) {
      animation.cur_frame = 0;
    }

    auto frame = strip.at(animation.cur_frame);

    sprite.src_x = static_cast<float>(frame.x) * sprite.src_w;
    sprite.src_y = static_cast<float>(frame.y) * sprite.src_h;
  }
}

// This could be refactored to be more "data-oriented"
// But since not many entities are actually animated I'll leave this for now
//
// Potential improvement - add benchmarks to test this function & monkey about to really see what improves
// performance
void Animation::execute_regular_animations(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Animation, components::Sprite>();

  for (auto [entity, animation, sprite] : view.each()) {
    if (!animation.playing) {
      continue;
    }

    auto strip = animation_strips.get().get(animation.strip_id);
    if (strip.empty()) {
      continue;
    }

    bool should_update_frame = false;

    animation.tick_counter++;
    if (animation.tick_counter >= animation.ticks_per_frame) {
      animation.tick_counter = 0;
      should_update_frame = true;

      if (animation.play_reversed) {
        if (animation.cur_frame == 0) {
          animation.cur_frame = strip.size() - 1;
        } else {
          animation.cur_frame--;
        }
      } else {
        animation.cur_frame++;
        if (animation.cur_frame >= strip.size()) {
          animation.cur_frame = 0;
        }
      }
    }

    ctx.ecs.replace<components::Animation>(entity, animation);

    if (should_update_frame) {
      auto frame = strip.at(animation.cur_frame);

      sprite.src_x = static_cast<float>(frame.x) * sprite.src_w;
      sprite.src_y = static_cast<float>(frame.y) * sprite.src_h;
    }
  }
}

} // namespace systems