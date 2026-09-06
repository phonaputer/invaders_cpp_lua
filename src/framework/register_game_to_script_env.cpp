#include "framework/register_game_to_script_env.hpp"
#include "framework/game.hpp"
#include "framework/script_environment.hpp"
#include <lualib.h>

namespace framework {

int get_scene_tick_count(lua_State *L) {
  luaL_checktype(L, lua_upvalueindex(1), LUA_TLIGHTUSERDATA);
  auto *game_ptr = static_cast<Game *>(lua_tolightuserdata(L, lua_upvalueindex(1)));

  auto result = game_ptr->get_scene_tick_count();

  lua_pushnumber(L, static_cast<lua_Number>(result));

  return 1;
}

void register_game_to_script_env(Game &game, ScriptEnvironment &scripts) {
  lua_State *L = &scripts.get_lua_state();

  auto *game_ptr = &game;

  lua_newtable(L);

  lua_pushlightuserdata(L, game_ptr);
  lua_pushcclosure(L, &get_scene_tick_count, "Scene.currentTick", 1);
  lua_setfield(L, -2, "currentTick");

  lua_setglobal(L, "Scene");
}

} // namespace framework