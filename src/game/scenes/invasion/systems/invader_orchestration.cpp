#include "game/scenes/invasion/systems/invader_orchestration.hpp"
#include "framework/animation_strip_registry.hpp"
#include "framework/constants.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/invader_animation.hpp"
#include "game/scenes/invasion/components/invader_orchestration_state.hpp"
#include "game/scenes/invasion/components/orchestrated_invader.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/constants.hpp"
#include "game/scenes/invasion/events/play_sound.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace systems {

InvaderOrchestration::InvaderOrchestration(
    framework::ScriptEnvironment &scripts,
    infra::CallbackGetter &callbacks,
    framework::AnimationStripRegistry &animation_strips
)
    : scripts{scripts},
      callbacks{callbacks},
      animation_strips{animation_strips} {
}

void InvaderOrchestration::execute(framework::ExecuteCtx &ctx) {
  if (ctx.ecs.ctx().contains<components::Pause>()) {
    return;
  }

  auto &state = ctx.ecs.ctx().get<components::InvaderOrchestrationState>();

  const auto invader_count = count_invaders(ctx);
  if (invader_count < 1) {
    invoke_no_invaders_callback(state);
    return;
  }

  shoot(state);

  if (!should_move_this_tick(state, invader_count)) {
    return;
  }

  play_arp(ctx, state);
  move_invaders(ctx, state, invader_count);
  animate_invaders(ctx);
}

size_t InvaderOrchestration::count_invaders(framework::ExecuteCtx &ctx) {
  return ctx.ecs.storage<components::OrchestratedInvader>().size();
}

void InvaderOrchestration::invoke_no_invaders_callback(components::InvaderOrchestrationState &state) {
  const auto maybe_callback = callbacks.get().get_callback(state.no_invaders_callback);
  if (maybe_callback.has_value()) {
    const auto &callback = maybe_callback.value();
    scripts.get().call_function(callback.package, callback.function);
  }
}

bool InvaderOrchestration::should_move_this_tick(
    components::InvaderOrchestrationState &state, const size_t invader_count
) {
  if (state.tick_counter >= state.base_ticks_per_move + invader_count) {
    state.tick_counter = 0;
    return true;
  }

  state.tick_counter++;

  return false;
}

void InvaderOrchestration::move_invaders(
    framework::ExecuteCtx &ctx, components::InvaderOrchestrationState &state, const size_t invader_count
) {
  const bool hit_wall = handle_wall_hit_if_any(ctx, state, invader_count);
  if (hit_wall) {
    return;
  }

  float dx = state.x_speed;
  if (invader_count == 1) {
    dx = state.last_invader_x_speed;
  }
  if (state.moving_left) {
    dx = -dx;
  }

  auto view = ctx.ecs.view<components::Position, components::OrchestratedInvader>();
  for (auto [entity, position] : view.each()) {
    position.x += dx;
  }
}

bool InvaderOrchestration::handle_wall_hit_if_any(
    framework::ExecuteCtx &ctx, components::InvaderOrchestrationState &state, const size_t invader_count
) {
  float dx = state.x_speed;
  if (invader_count == 1) {
    dx = state.last_invader_x_speed;
  }

  bool hit_wall = false;
  auto view = ctx.ecs.view<components::Position, components::OrchestratedInvader>();

  if (state.moving_left) {
    dx = -dx;
    for (auto [entity, position] : view.each()) {
      if (position.x + dx <= 0) {
        hit_wall = true;
        break;
      }
    }
  } else {
    for (auto [entity, position] : view.each()) {
      if (position.x + position.w + dx >= framework::WINDOW_WIDTH) {
        hit_wall = true;
        break;
      }
    }
  }

  if (!hit_wall) {
    return false;
  }

  state.moving_left = !state.moving_left;

  float dy = state.y_speed;
  if (invader_count == 1) {
    dy = state.last_invader_y_speed;
  }

  for (auto [entity, position] : view.each()) {
    position.y += dy;
  }

  bool touched_down = false;
  for (auto [entity, position] : view.each()) {
    if (position.y + position.h >= invasion::GROUND_HEIGHT) {
      touched_down = true;
      break;
    }
  }

  if (touched_down) {
    invoke_touchdown_callback(state);
  }

  return true;
}

void InvaderOrchestration::invoke_touchdown_callback(components::InvaderOrchestrationState &state) {
  const auto maybe_callback = callbacks.get().get_callback(state.touchdown_callback);
  if (maybe_callback.has_value()) {
    const auto &callback = maybe_callback.value();
    scripts.get().call_function(callback.package, callback.function);
  }
}

void InvaderOrchestration::animate_invaders(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::InvaderAnimation, components::Sprite>();

  for (auto [entity, animation, sprite] : view.each()) {
    auto strip = animation_strips.get().get(animation.strip_id);

    animation.cur_frame = (animation.cur_frame + 1) % strip.size();

    auto frame = strip.at(animation.cur_frame);
    sprite.src_x = static_cast<float>(frame.x) * sprite.src_w;
    sprite.src_y = static_cast<float>(frame.y) * sprite.src_h;
  }
}

void InvaderOrchestration::shoot(components::InvaderOrchestrationState &state) {
  if (state.shoot_tick_counter < state.ticks_per_shot) {
    state.shoot_tick_counter++;
    return;
  }

  state.shoot_tick_counter = 0;

  const auto maybe_callback = callbacks.get().get_callback(state.shoot_callback);
  if (maybe_callback.has_value()) {
    const auto &callback = maybe_callback.value();
    scripts.get().call_function(callback.package, callback.function);
  }
}

// Would be nice to add this from Luau, but I'm lazy so I'm hardcoding it
std::vector<std::string> arp_sounds() {
  const std::vector<std::string> arp = {"arp1.wav", "arp2.wav", "arp3.wav", "arp4.wav"};
  return arp;
}

void InvaderOrchestration::play_arp(
    framework::ExecuteCtx &ctx, components::InvaderOrchestrationState &state
) {
  const auto arp = arp_sounds();

  ctx.events.push_back_draw<events::PlaySound>(events::PlaySound{.audio_src = arp.at(state.cur_arp)});

  state.cur_arp++;
  if (state.cur_arp >= arp.size()) {
    state.cur_arp = 0;
  }
}

} // namespace systems