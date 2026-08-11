#include "game/scenes/invasion/systems/player_movement.hpp"
#include "framework/constants.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include <algorithm>

namespace systems {

void PlayerMovement::execute(framework::ExecuteCtx &ctx) {
  if (ctx.ecs.ctx().contains<components::Pause>()) {
    return;
  }

  auto view = ctx.ecs.view<components::PlayerMovement, components::Position, components::Animation>();

  for (auto [entity, movement, position, animation] : view.each()) {
    if (ctx.player_input.is_engaged(framework::PlayerInput::LEFT)
        == ctx.player_input.is_engaged(framework::PlayerInput::RIGHT)) {
      animation.playing = false;
      continue;
    }

    if (ctx.player_input.is_engaged(framework::PlayerInput::LEFT)) {
      auto new_x = position.x - static_cast<float>(movement.x_speed);
      new_x = std::max(new_x, 0.0F);

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = true;
      } else {
        animation.playing = false;
      }

      position.x = new_x;
    }

    if (ctx.player_input.is_engaged(framework::PlayerInput::RIGHT)) {
      auto new_x = position.x + static_cast<float>(movement.x_speed);
      if (new_x + position.w > framework::WINDOW_WIDTH) {
        new_x = framework::WINDOW_WIDTH - position.w;
      }

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = false;
      } else {
        animation.playing = false;
      }

      position.x = new_x;
    }
  }
}

} // namespace systems