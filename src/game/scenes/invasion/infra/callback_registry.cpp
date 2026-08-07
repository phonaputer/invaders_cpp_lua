#include "game/scenes/invasion/infra/callback_registry.hpp"

#include "framework/script_environment.hpp"
#include <cstdint>
#include <mutex>
#include <optional>
#include <string>

namespace infra {

CallbackID CallbackRegistry::get_next_id() {
  const std::scoped_lock lock(cur_id_lock);
  return ++cur_id;
}

CallbackID CallbackRegistry::register_callback(const std::string &package, const std::string &function) {
  auto new_id = get_next_id();

  callbacks.insert_or_assign(new_id, Callback{.package = package, .function = function});

  return new_id;
}

std::optional<Callback> CallbackRegistry::get_callback(CallbackID id) const {
  if (!callbacks.contains(id)) {
    return std::nullopt;
  }

  return callbacks.at(id);
}

void add_callback_registry_to_script_env(framework::ScriptEnvironment &scripts, CallbackRegistry &callbacks) {
  scripts.register_function(
      "Callbacks",
      "register",
      [&callbacks](const std::string &package, const std::string &function) -> uint16_t {
        return callbacks.register_callback(package, function);
      }
  );
}

} // namespace infra