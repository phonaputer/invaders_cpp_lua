#include "game/scenes/invasion/systems/player_attack.hpp"

#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/player_attack.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/infra/weapon_registry.hpp"
#include <entt.hpp>

namespace systems {

PlayerAttack::PlayerAttack(
    framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks, infra::WeaponGetter &weapons
)
    : scripts{scripts},
      callbacks{callbacks},
      weapons{weapons} {
}

void PlayerAttack::execute(framework::ExecuteCtx &ctx) {
  if (ctx.ecs.ctx().contains<components::Pause>()) {
    return;
  }

  auto view = ctx.ecs.view<components::PlayerAttack, components::Position>();

  for (auto [entity, attack, position] : view.each()) {
    const auto maybe_weapon = weapons.get().get_weapon(attack.weapon);
    if (!maybe_weapon.has_value()) {
      continue;
    }

    const auto weapon = maybe_weapon.value();

    if (attack.tick_counter >= weapon.ticks_per_shot
        && ctx.player_input.is_engaged(framework::PlayerInput::FIRE)) {
      attack.tick_counter = 0;

      auto maybe_callback = callbacks.get().get_callback(weapon.shoot_callback);
      if (maybe_callback.has_value()) {
        const auto &callback = maybe_callback.value();
        scripts.get().call_function(
            callback.package, callback.function, entt::to_integral(entity), position.x, position.y
        );
      }
    } else {
      attack.tick_counter++;
    }
  }
}

} // namespace systems