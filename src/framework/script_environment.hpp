#pragma once

#include "framework/animation_strip_registry.hpp"
#include <entt.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <luacode.h>
#include <lualib.h>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>

#include <LuaBridge/LuaBridge.h>

namespace framework {

struct LuaStateDeleter {
    void operator()(lua_State *L_p) const {
      lua_close(L_p);
    }
};

class ScriptEnvironment {
  private:
    std::reference_wrapper<entt::registry> ecs;
    std::unique_ptr<lua_State, LuaStateDeleter> L;

    std::stack<std::string> executing_script;
    std::unordered_map<std::string, luabridge::LuaRef> function_cache;

    std::optional<luabridge::LuaRef> get_function(const std::string &name_space, const std::string &function);
    luabridge::LuaRef luau_require(lua_State *local_L);
    bool open_and_run_file(lua_State *local_L, int num_results, const std::string &path);

    static std::string get_require_path(const std::string &require_target, const std::string &current_script);

  public:
    // TODO consider moving registration of these types into Luau out of this file
    explicit ScriptEnvironment(entt::registry &ecs, AnimationStripRegistry &animation_strips);

    void exec_script_file(const std::string &path);
    void exec_package(const std::string &path);
    template <typename T, typename F>
    void register_component(const std::string &name, F &&fields_register_func);
    template <typename F> void register_function(const std::string &name, F &&func);
    template <typename Result = void, typename... Args>
    Result call_global_function(const std::string &name, Args &&...args);
    template <typename Result = void, typename... Args>
    Result call_function(const std::string &name_space, const std::string &function, Args &&...args);
};

} // namespace framework

#include "framework/script_environment.tpp"