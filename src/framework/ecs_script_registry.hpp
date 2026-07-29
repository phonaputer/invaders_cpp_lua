#pragma once

#include <cinttypes>
#include <entt.hpp>
#include <functional>
#include <lualib.h>
#include <unordered_map>

#include <LuaBridge/LuaBridge.h>

namespace framework {

// TODO - consider tying the lifetime of lua_State L to this class & moving
// caching_script_runner logic here

// No need to use an abstract class "interface" here since this will only be
// called by plumbing that won't be unit tested.
class ECSScriptRegistry {
  private:
    entt::registry &ecs;
    lua_State &L;

    uint32_t cur_type_id = 0;
    std::unordered_map<uint32_t, std::function<void(entt::registry &, entt::runtime_view &)>> type_id_to_view_func;

    uint32_t create_entity();
    luabridge::LuaRef generate_view_callback(luabridge::LuaRef table);

  public:
    ECSScriptRegistry(entt::registry &ecs, lua_State &L);

    // Looking forward to C++26 reflection when hopefully the string & function params can be removed.
    template <typename T, typename F> void register_component(std::string name, F &&fields_register_func);
};

} // namespace framework

#include "framework/ecs_script_registry.tpp"