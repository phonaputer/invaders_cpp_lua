#include "game/scenes/invasion/systems/callback_on_timeout.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/callback_on_timeout.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <entt.hpp>
#include <vector>

namespace systems {

CallbackOnTimeout::CallbackOnTimeout(framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks)
    : scripts{scripts},
      callbacks{callbacks} {
}

void CallbackOnTimeout::execute(framework::ExecuteCtx &ctx) {
  std::vector<entt::entity> to_delete;

  auto view = ctx.ecs.view<components::CallbackOnTimeout>();
  for (auto [entity, cb_on_timeout] : view.each()) {
    if (cb_on_timeout.tick_counter >= cb_on_timeout.timeout_ticks) {
      to_delete.push_back(entity);
      const auto maybe_callback = callbacks.get().get_callback(cb_on_timeout.callback);
      if (maybe_callback.has_value()) {
        const auto &callback = maybe_callback.value();
        scripts.get().call_function(callback.package, callback.function);
      }
    } else {
      cb_on_timeout.tick_counter++;
    }
  }

  for (const auto &entity : to_delete) {
    ctx.ecs.destroy(entity);
  }
}

} // namespace systems