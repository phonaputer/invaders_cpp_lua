#pragma once

#include <entt.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <luacode.h>
#include <lualib.h>
#include <memory>
#include <sstream>
#include <string>

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

    void open_and_run_file(const std::string &path);

  public:
    explicit ScriptEnvironment(entt::registry &ecs);
    void exec_script_file(const std::string &path);
    void exec_all_script_files_in_dir(const std::string &path);
    template <typename T, typename F> void register_component(const std::string &name, F &&fields_register_func);
    template <typename F> void register_function(const std::string &name, F &&func);
    template <typename Result = void, typename... Args> Result call_function(const std::string &name, Args &&...args);
};

} // namespace framework

#include "framework/script_environment.tpp"