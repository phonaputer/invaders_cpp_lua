#include "game/scenes/invasion/infra/weapon_registry.hpp"

#include "framework/script_environment.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>
#include <lua.h>
#include <lualib.h>
#include <mutex>
#include <optional>

namespace infra {

WeaponID WeaponRegistry::get_next_id() {
  const std::scoped_lock lock(cur_id_lock);
  return ++cur_id;
}

WeaponID WeaponRegistry::register_weapon(Weapon weapon) {
  auto new_id = get_next_id();

  weapons.insert_or_assign(new_id, weapon);

  return new_id;
}

std::optional<Weapon> WeaponRegistry::get_weapon(WeaponID id) const {
  if (!weapons.contains(id)) {
    return std::nullopt;
  }

  return weapons.at(id);
}

int register_weapon(lua_State *L) {
  luaL_checktype(L, lua_upvalueindex(1), LUA_TLIGHTUSERDATA);
  auto *weapons_ptr = static_cast<WeaponRegistry *>(lua_tolightuserdata(L, lua_upvalueindex(1)));

  luaL_checktype(L, 1, LUA_TTABLE);

  lua_getfield(L, 1, "ticksPerShot");
  if (lua_isnumber(L, -1) == 0) {
    luaL_argerror(L, 2, "Expected 'ticksPerShot' field to be a number");
  }
  const auto ticks_per_shot = static_cast<uint16_t>(lua_tonumber(L, -1));
  lua_pop(L, 1);

  lua_getfield(L, 1, "shootCallback");
  if (lua_isnumber(L, -1) == 0) {
    luaL_argerror(L, 2, "Expected 'shootCallback' field to be a number");
  }
  const auto shoot_callback = static_cast<CallbackID>(lua_tonumber(L, -1));
  lua_pop(L, 1);

  auto weapon_id = weapons_ptr->register_weapon(
      Weapon{
          .ticks_per_shot = ticks_per_shot,
          .shoot_callback = shoot_callback,
      }
  );

  lua_pushnumber(L, static_cast<lua_Number>(weapon_id));

  return 1;
}

void add_weapon_registry_to_script_env(framework::ScriptEnvironment &scripts, WeaponRegistry &weapons) {
  lua_State &L_ref = scripts.get_lua_state();
  lua_State *L = &L_ref;

  lua_newtable(L);

  lua_pushlightuserdata(L, &weapons);
  lua_pushcclosure(L, &register_weapon, "Weapons.register", 1);

  lua_setfield(L, -2, "register");
  lua_setglobal(L, "Weapons");
}

} // namespace infra