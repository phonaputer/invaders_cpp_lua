#pragma once

#include "framework/script_environment.hpp"
#include <cstdint>

#include <LuaBridge/LuaBridge.h>

namespace components {

enum class DamageType : uint8_t {
  Alien = 1,
  Player = 1 << 1,
  Fortress = 1 << 2,
  Alien_Projectile = 1 << 3,
  Player_Projectile = 1 << 4
};

using DamageTypeSet = uint8_t;

constexpr DamageTypeSet operator|(DamageType l, DamageType r) {
  return static_cast<DamageTypeSet>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}

constexpr DamageTypeSet operator|(DamageTypeSet l, DamageType r) {
  return static_cast<DamageTypeSet>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}

inline void register_damage_type_enum_to_script_env(framework::ScriptEnvironment &scripts) {
  lua_State &L = scripts.get_lua_state();

  luabridge::getGlobalNamespace(&L)
      .beginNamespace("DamageType")
      .addProperty("ALIEN", []() { return static_cast<uint8_t>(DamageType::Alien); })
      .addProperty("PLAYER", []() { return static_cast<uint8_t>(DamageType::Player); })
      .addProperty("FORTRESS", []() { return static_cast<uint8_t>(DamageType::Fortress); })
      .addProperty("ALIEN_PROJECTILE", []() { return static_cast<uint8_t>(DamageType::Alien_Projectile); })
      .addProperty("PLAYER_PROJECTILE", []() { return static_cast<uint8_t>(DamageType::Player_Projectile); })
      .endNamespace();
}

} // namespace components