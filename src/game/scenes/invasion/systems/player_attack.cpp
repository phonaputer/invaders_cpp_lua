#include "game/scenes/invasion/systems/player_attack.hpp"

#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/player_attack.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include <entt.hpp>

namespace systems {

PlayerAttack::PlayerAttack(framework::ScriptEnvironment &scripts)
    : scripts{scripts} {
}

void PlayerAttack::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::PlayerAttack, components::Position>();

  for (auto [entity, attack, position] : view.each()) {
    if (attack.tick_counter >= attack.ticks_per_attack
        && ctx.player_input.is_engaged(framework::PlayerInput::FIRE)) {
      attack.tick_counter = 0;
      scripts.get().call_function(
          attack.callback_package, attack.callback_function, entt::to_integral(entity), position.x, position.y
      );
    } else {
      attack.tick_counter++;
    }
  }
}

} // namespace systems