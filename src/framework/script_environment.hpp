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

struct GetFunctionArgs {
    bool is_global;
    std::string package;
    std::string function;
};

// TODO
// . Add hot reloading.
// . Add registry of callbacks so that entites don't need to contain strings.
// . Think about the lifetime of this type relative to a scene.
class ScriptEnvironment {
  private:
    std::unique_ptr<lua_State, LuaStateDeleter> L;

    std::stack<std::string> executing_script;
    std::unordered_map<std::string, luabridge::LuaRef> function_cache;
    std::unordered_map<std::string, luabridge::LuaRef> global_function_cache;
    std::unordered_map<std::string, luabridge::LuaRef> package_cache;

    bool open_and_run_file(lua_State *local_L, int num_results, const std::string &path);
    bool exec_source(lua_State *local_L, int num_results, const std::string &path, const std::string &source);

    std::optional<luabridge::LuaRef> get_function(const std::string &package, const std::string &function);
    std::optional<luabridge::LuaRef> get_global_function(const std::string &function);

    std::optional<luabridge::LuaRef> load_package(const std::string &package);
    std::optional<luabridge::LuaRef> get_package(const std::string &package);

    luabridge::LuaRef luau_require(lua_State *local_L);
    static std::string get_require_path(const std::string &require_target, const std::string &current_script);

  public:
    ScriptEnvironment();

    // Execute any script file.
    //
    // The input path should be relative to the "scripts/" directory in the repo root.
    // The ".luau" extension should be excluded.
    //
    // For example, the input "example/test" will run the file "scripts/example/test.luau".
    void exec_script_file(const std::string &path);

    // Execute a string containing Luau code.
    // This is primarily intended for unit testing.
    //
    // If the script returns a table, the optional "package" parameter must be set.
    // Then any functions in the table can be invoked by calling call_function with the same package name.
    void exec_script_string(const std::string &source, const std::string &package = "");

    // Call a function in Luau's global namespace.
    //
    // The "name" argument may reference nested tables. For example, if the global namespace includes...
    //
    //    Example: {
    //       Test: {
    //          add: (a: number, b: number) -> (number)
    //       }
    //    }
    //    subtract: (a: number, b:number) -> (number)
    //
    // ... then `call_global_function<lua_Number>("Example.Test.add", 2, 3)` will call the nested add
    // function. And `call_global_function<lua_Number>("subtract", 2, 3)` will return the top-level subtract
    // function.
    template <typename Result = void, typename... Args>
    Result call_global_function(const std::string &name, Args &&...args);

    // Call a function within a Luau package.
    //
    // This function executes the "init.luau" file in the specified package and calls a function in the
    // returned table.
    //
    // The "package" input should be a path relative to the "scripts/" directory in the repo root.
    // The "init.luau" filename should be excluded.
    //
    // For example, the input "example/test" will execute the file "scripts/example/test/init.luau".
    //
    // Similarly to "call_global_function", "name" may reference nested tables.
    template <typename Result = void, typename... Args>
    Result call_function(const std::string &package, const std::string &name, Args &&...args);

    // Register a component to the Luau environment so that Luau may perform CRUD operations on this component
    // in the EnTT ECS.
    template <typename T, typename F>
    void register_component(entt::registry &ecs, const std::string &name, F &&fields_register_func);

    // Register a C++ function so that it may be called from Luau.
    //
    // All functions registered here will be added to a global table named "Host".
    template <typename F> void register_function(const std::string &name, F &&func);

    // Get the Luau state embeded in the environment.
    //
    // For when it's necessary do something in the Luau state not supported by the above helpers.
    lua_State &get_lua_state();
};

// Intentionally using references here since this is a parameter object.
//
// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct SceneComponents {
    ScriptEnvironment &scripts;
    entt::registry &ecs;
    AnimationStripRegistry &animation_strips;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

void register_scene_components_to_script_env(SceneComponents args);

} // namespace framework

#include "framework/script_environment.tpp"