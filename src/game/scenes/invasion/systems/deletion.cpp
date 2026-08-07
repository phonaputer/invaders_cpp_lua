#include "game/scenes/invasion/systems/deletion.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/deletion_callback.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/ttl.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace systems {

Deletion::Deletion(framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks)
    : scripts{scripts},
      callbacks{callbacks} {
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
    auto maybe_callback = callbacks.get().get_callback(callback.callback);

    if (maybe_callback.has_value()) {
      scripts.get().call_function(maybe_callback.value().package, maybe_callback.value().function);
    }
  }

  auto deletion_view = ctx.ecs.view<components::ToBeDeleted>();
  ctx.ecs.destroy(deletion_view.begin(), deletion_view.end());
}

} // namespace systems