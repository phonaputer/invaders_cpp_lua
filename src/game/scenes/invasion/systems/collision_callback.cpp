#include "game/scenes/invasion/systems/collision_callback.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <entt.hpp>

namespace systems {

CollisionCallback::CollisionCallback(framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks)
    : scripts{scripts},
      callbacks{callbacks} {
}

void CollisionCallback::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all<events::CollisionOccurred>()) {
    if (ctx.ecs.all_of<components::Collision>(event.who_am_i)) {
      const auto &collision = ctx.ecs.get<components::Collision>(event.who_am_i);
      const auto maybe_callback = callbacks.get().get_callback(collision.callback);
      if (maybe_callback.has_value()) {
        const auto &callback = maybe_callback.value();
        scripts.get().call_function(
            callback.package,
            callback.function,
            entt::to_integral(event.who_am_i),
            entt::to_integral(event.who_i_hit)
        );
      }
    }
  }
}

} // namespace systems