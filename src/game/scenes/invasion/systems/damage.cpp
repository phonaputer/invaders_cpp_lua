#include "game/scenes/invasion/systems/damage.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/damage.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"

namespace systems {

void Damage::execute(framework::ExecuteCtx &ctx) {
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

    hitpoints->cur_hitpoints -= damage->amount;

    if (hitpoints->cur_hitpoints < 1) {
      ctx.ecs.emplace<components::ToBeDeleted>(collision.who_i_hit);
    }
  }
}

} // namespace systems