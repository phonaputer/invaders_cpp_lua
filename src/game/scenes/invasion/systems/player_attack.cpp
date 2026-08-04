#include "game/scenes/invasion/systems/player_attack.hpp"

#include "framework/system.hpp"
#include "game/scenes/invasion/components/player_attack.hpp"
#include "game/scenes/invasion/components/position.hpp"

namespace systems {

void PlayerAttack::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::PlayerAttack, components::Position>();

  for (auto [entity, attack, position] : view.each()) {
    if (attack.tick_counter >= attack.ticks_per_attack) {
      attack.tick_counter = 0;
      ctx.scripts.call_function("invasion", attack.callback, position.x, position.y);
    } else {
      attack.tick_counter++;
    }
  }
}

} // namespace systems