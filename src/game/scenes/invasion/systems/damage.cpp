#include "game/scenes/invasion/systems/damage.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/damage.hpp"
#include "game/scenes/invasion/components/damage_callback.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"
#include <entt.hpp>
#include <vector>

namespace systems {

Damage::Damage(framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks)
    : scripts{scripts},
      callbacks{callbacks} {
}

void Damage::execute(framework::ExecuteCtx &ctx) {
  std::vector<entt::entity> damaged_entities;

  for (const auto &collision : ctx.events.get_all<events::CollisionOccurred>()) {
    auto *hitpoints = ctx.ecs.try_get<components::Hitpoints>(collision.who_i_hit);
    if (hitpoints == nullptr) {
      continue;
    }

    auto *damage = ctx.ecs.try_get<components::Damage>(collision.who_am_i);
    if (damage == nullptr) {
      continue;
    }

    const bool can_damage = (damage->type & hitpoints->susceptible_to) > 0;
    if (!can_damage) {
      continue;
    }

    damaged_entities.push_back(collision.who_i_hit);

    hitpoints->cur_hitpoints -= damage->amount;

    if (hitpoints->cur_hitpoints < 1) {
      ctx.ecs.emplace<components::ToBeDeleted>(collision.who_i_hit);
    }
  }

  for (const auto &entity : damaged_entities) {
    if (ctx.ecs.all_of<components::DamageCallback>(entity)) {
      auto component = ctx.ecs.get<components::DamageCallback>(entity);

      auto maybe_callback = callbacks.get().get_callback(component.callback);
      if (maybe_callback.has_value()) {
        const auto &callback = maybe_callback.value();
        scripts.get().call_function(callback.package, callback.function, entt::to_integral(entity));
      }
    }
  }
}

} // namespace systems