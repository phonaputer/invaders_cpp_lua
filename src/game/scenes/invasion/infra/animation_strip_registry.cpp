#include "game/scenes/invasion/infra/animation_strip_registry.hpp"
#include <cassert>

namespace infra {

AnimationStripID AnimationStripRegistry::create() {
  auto result = ++cur_id;

  animations.try_emplace(result);

  return result;
}

void AnimationStripRegistry::add_frame(AnimationStripID id, AnimationFrame frame) {
  assert(animations.contains(id) && "AnimationStrip to add frame to does not exist");
  if (animations.contains(id)) {
    animations.at(id).push_back(frame);
  }
}

AnimationStrip AnimationStripRegistry::get(AnimationStripID id) const {
  assert(animations.contains(id) && "Requested AnimationStrip does not exist");
  if (animations.contains(id)) {
    return animations.at(id);
  }

  return {};
}

int create_animation_strip(lua_State *L) {
  luaL_checktype(L, lua_upvalueindex(1), LUA_TLIGHTUSERDATA);
  auto *animation_strips_ptr
      = static_cast<AnimationStripRegistry *>(lua_tolightuserdata(L, lua_upvalueindex(1)));

  auto strip_id = animation_strips_ptr->create();

  lua_pushnumber(L, static_cast<lua_Number>(strip_id));

  return 1;
}

int add_frame(lua_State *L) {
  luaL_checktype(L, lua_upvalueindex(1), LUA_TLIGHTUSERDATA);
  auto *animation_strips_ptr
      = static_cast<AnimationStripRegistry *>(lua_tolightuserdata(L, lua_upvalueindex(1)));

  auto strip_id = static_cast<AnimationStripID>(luaL_checknumber(L, 1));
  auto x = static_cast<int>(luaL_checknumber(L, 2));
  auto y = static_cast<int>(luaL_checknumber(L, 3));

  animation_strips_ptr->add_frame(strip_id, {.x = x, .y = y});

  return 0;
}

void add_animation_strip_registry_to_script_env(
    framework::ScriptEnvironment &scripts, AnimationStripRegistry &animation_strips
) {
  lua_State &L_ref = scripts.get_lua_state();
  lua_State *L = &L_ref;

  lua_newtable(L);

  lua_pushlightuserdata(L, &animation_strips);
  lua_pushcclosure(L, &create_animation_strip, "AnimationStrips.create", 1);
  lua_setfield(L, -2, "create");

  lua_pushlightuserdata(L, &animation_strips);
  lua_pushcclosure(L, &add_frame, "AnimationStrips.addFrame", 1);
  lua_setfield(L, -2, "addFrame");

  lua_setglobal(L, "AnimationStrips");
}

} // namespace infra