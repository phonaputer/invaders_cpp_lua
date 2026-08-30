#include "game/scenes/invasion/infra/callback_registry.hpp"

#include "framework/script_environment.hpp"
#include <lua.h>
#include <lualib.h>
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

int register_callback(lua_State *L) {
  luaL_checktype(L, lua_upvalueindex(1), LUA_TLIGHTUSERDATA);
  auto *callbacks_ptr = static_cast<CallbackRegistry *>(lua_tolightuserdata(L, lua_upvalueindex(1)));

  const std::string package = luaL_checkstring(L, 1);
  const std::string function = luaL_checkstring(L, 2);

  auto callback_id = callbacks_ptr->register_callback(package, function);

  lua_pushnumber(L, static_cast<lua_Number>(callback_id));

  return 1;
}

void add_callback_registry_to_script_env(framework::ScriptEnvironment &scripts, CallbackRegistry &callbacks) {
  lua_State *L = &scripts.get_lua_state();

  lua_newtable(L);

  lua_pushlightuserdata(L, &callbacks);
  lua_pushcclosure(L, &register_callback, "Callbacks.register", 1);
  lua_setfield(L, -2, "register");

  lua_setglobal(L, "Callbacks");
}

} // namespace infra