#pragma once

#include "framework/script_environment.hpp"
#include <cstdint>

#include <LuaBridge/LuaBridge.h>

namespace components {

enum class DamageType : uint8_t { Alien, Player, Fortress, Alien_Projectile, Player_Projectile };

inline uint8_t damage_type_to_index(DamageType dt) {
  return static_cast<uint8_t>(1) << static_cast<uint8_t>(dt);
}

using DamageTypeSet = uint8_t;

inline void register_damage_type_enum_to_script_env(framework::ScriptEnvironment &scripts) {
  lua_State &L = scripts.get_lua_state();

  luabridge::getGlobalNamespace(&L)
      .beginNamespace("DamageType")
      .addProperty("ALIEN", []() { return damage_type_to_index(DamageType::Alien); })
      .addProperty("PLAYER", []() { return damage_type_to_index(DamageType::Player); })
      .addProperty("FORTRESS", []() { return damage_type_to_index(DamageType::Fortress); })
      .addProperty("ALIEN_PROJECTILE", []() { return damage_type_to_index(DamageType::Alien_Projectile); })
      .addProperty("PLAYER_PROJECTILE", []() { return damage_type_to_index(DamageType::Player_Projectile); })
      .endNamespace();
}

} // namespace components