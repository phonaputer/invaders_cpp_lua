#pragma once

#include "framework/script_environment.hpp"
#include <cstdint>
#include <lualib.h>

namespace components {

enum class DamageType : uint8_t {
  Alien = 1U,
  Player = 1U << 1U,
  Fortress = 1U << 2U,
  Alien_Projectile = 1U << 3U,
  Player_Projectile = 1U << 4U
};

using DamageTypeSet = uint8_t;

constexpr DamageTypeSet operator|(DamageType l, DamageType r) {
  return static_cast<DamageTypeSet>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}

constexpr DamageTypeSet operator|(DamageTypeSet l, DamageType r) {
  return static_cast<DamageTypeSet>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}

inline void register_damage_type_enum_to_script_env(framework::ScriptEnvironment &scripts) {
  lua_State *L = &scripts.get_lua_state();

  lua_newtable(L);

  lua_pushnumber(L, static_cast<lua_Number>(DamageType::Alien));
  lua_setfield(L, -2, "ALIEN");

  lua_pushnumber(L, static_cast<lua_Number>(DamageType::Player));
  lua_setfield(L, -2, "PLAYER");

  lua_pushnumber(L, static_cast<lua_Number>(DamageType::Fortress));
  lua_setfield(L, -2, "FORTRESS");

  lua_pushnumber(L, static_cast<lua_Number>(DamageType::Alien_Projectile));
  lua_setfield(L, -2, "ALIEN_PROJECTILE");

  lua_pushnumber(L, static_cast<lua_Number>(DamageType::Player_Projectile));
  lua_setfield(L, -2, "PLAYER_PROJECTILE");

  lua_setglobal(L, "DamageType");
}

} // namespace components