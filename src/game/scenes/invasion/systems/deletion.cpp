#include "game/scenes/invasion/systems/deletion.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/deletion_callback.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/ttl.hpp"

namespace systems {

Deletion::Deletion(framework::ScriptEnvironment &scripts)
    : scripts{scripts} {
}

void Deletion::execute(framework::ExecuteCtx &ctx) {
  auto ttl_view = ctx.ecs.view<components::TTL>();
  for (auto [entity, ttl] : ttl_view.each()) {
    if (ttl.tick_counter >= ttl.ticks_to_live) {
      ctx.ecs.emplace<components::ToBeDeleted>(entity);
    } else {
      ttl.tick_counter++;
    }
  }

  auto callback_view = ctx.ecs.view<components::ToBeDeleted, components::DeletionCallback>();
  for (auto [entity, callback] : callback_view.each()) {
    scripts.get().call_function(callback.package, callback.callback);
  }

  auto deletion_view = ctx.ecs.view<components::ToBeDeleted>();
  ctx.ecs.destroy(deletion_view.begin(), deletion_view.end());
}

} // namespace systems