#pragma once

#include "framework/constants.hpp"
#include <entt.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lua.h>
#include <luacode.h>
#include <lualib.h>
#include <memory>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

#include <LuaBridge/LuaBridge.h>

namespace framework {

constexpr std::string CPP_NAMESPACE = "Host";
constexpr std::string SCRIPT_PATH_PREFIX = "scripts/";
constexpr std::string SCRIPT_PATH_SUFFIX = ".luau";
constexpr std::string PACKAGE_ENTRYPOINT_FILE = "/init.luau";

struct FreeDeleter {
    void operator()(void *ptr) const {
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory, cppcoreguidelines-no-malloc, hicpp-no-malloc)
      std::free(ptr);
    }
};

inline ScriptEnvironment::ScriptEnvironment()
    : L{luaL_newstate(), LuaStateDeleter()} {
  luaL_openlibs(L.get());
  luabridge::getGlobalNamespace(L.get()).addFunction("require", [this](lua_State *local_L) {
    return luau_require(local_L);
  });
}

inline luabridge::LuaRef ScriptEnvironment::luau_require(lua_State *local_L) {
  if (executing_script.empty()) {
    std::cerr << "ScriptEnvironment: Tried to call require outside loading a Luau file.\n";
    assert(false && "Tried to call require outside loading a Luau file.");
    return {local_L, false};
  }

  if (lua_isstring(local_L, 1) != 1) {
    std::cerr << "ScriptEnvironment: Tried to require non-string while executing: " << executing_script.top()
              << "\n";
    assert(false && "Tried to require non-string.");
    return {local_L, false};
  }

  const std::string file_to_execute = get_require_path(lua_tostring(local_L, 1), executing_script.top());
  if (!file_to_execute.starts_with(SCRIPT_PATH_PREFIX)) {
    std::cerr << "ScriptEnvironment: Disallowed require path: " << file_to_execute << "\n";
    assert(false && "Disallowed require path.");
    return {local_L, false};
  }

  const bool success = open_and_run_file(local_L, 1, file_to_execute);
  if (!success) {
    return {local_L, false};
  }

  auto result = luabridge::LuaRef::fromStack(local_L, -1);

  lua_pop(local_L, 1);

  return result;
}

inline std::string
ScriptEnvironment::get_require_path(const std::string &require_target, const std::string &current_script) {
  std::string file_name_str = require_target;
  std::filesystem::path base_dir(current_script);

  // Details on this special case for "init.luau" files may be found in this spec:
  // https://rfcs.luau.org/abstract-module-paths-and-init-dot-luau.html
  //
  // Dunno why Luau feels the need to have this, but I implemented it.
  if (current_script.ends_with(PACKAGE_ENTRYPOINT_FILE)) {
    if (file_name_str.starts_with("@self")) {
      file_name_str.replace(0, 5, ".");
    } else {
      base_dir = base_dir.parent_path();
    }
  }

  const std::filesystem::path file_name(file_name_str);

  auto result = (base_dir.parent_path() / file_name).lexically_normal().string();

  std::error_code ec;
  if (std::filesystem::is_directory(result, ec)) {
    result += PACKAGE_ENTRYPOINT_FILE;
  } else {
    result += SCRIPT_PATH_SUFFIX;
  }

  return result;
}

inline bool
ScriptEnvironment::open_and_run_file(lua_State *local_L, int num_results, const std::string &path) {
  std::ifstream script_file(path);
  if (!script_file.is_open()) {
    std::cerr << "ScriptEnvironment: Failed to open script file: " << path << "\n";
    assert(script_file.is_open() && "Failed to open script file.");
    return false;
  }

  std::stringstream script_buffer;
  script_buffer << script_file.rdbuf();
  assert(script_buffer.str().length() > 0 && "Loaded empty script.");

  const std::string source = script_buffer.str();

  return exec_source(local_L, num_results, path, source);
}

inline bool ScriptEnvironment::exec_source(
    lua_State *local_L, int num_results, const std::string &path, const std::string &source
) {
  size_t bytecode_size = 0;
  const std::unique_ptr<char, FreeDeleter> bytecode{
      luau_compile(source.c_str(), source.length(), nullptr, &bytecode_size), FreeDeleter()
  };

  auto result = luau_load(local_L, path.c_str(), bytecode.get(), bytecode_size, 0);
  if (result != LUA_OK) {
    std::cerr << "ScriptEnvironment: Failed to load bytecode '" << path << "': " << lua_tostring(local_L, -1)
              << "\n";
    assert(false && "Failed to load Luau bytecode.");
    return false;
  }

  executing_script.push(path);
  result = lua_pcall(local_L, 0, num_results, 0);
  executing_script.pop();

  if (result != LUA_OK) {
    std::cerr << "ScriptEnvironment: Failed to execute script '" << path << "': " << lua_tostring(local_L, -1)
              << "\n";
    lua_pop(local_L, 1);
    assert(false && "Failed to run Luau script.");
    return false;
  }

  return true;
}

inline void ScriptEnvironment::exec_script_file(const std::string &path) {
  open_and_run_file(L.get(), 0, SCRIPT_PATH_PREFIX + path + SCRIPT_PATH_SUFFIX);
}

inline void ScriptEnvironment::exec_script_string(const std::string &source, const std::string &package) {
  const bool is_package = !package.empty();

  int num_results = 0;
  if (is_package) {
    num_results = 1;
  }
  auto success = exec_source(L.get(), num_results, "raw-string", source);
  if (!success) {
    return;
  }

  if (is_package) {
    auto result = luabridge::LuaRef::fromStack(L.get(), -1);
    lua_pop(L.get(), 1);
    if (!result.isTable()) {
      std::cerr << "ScriptEnvironment: String script did not return a table.\n";
      return;
    }

    package_cache.insert_or_assign(package, result);
  }
}

inline std::optional<luabridge::LuaRef> ScriptEnvironment::load_package(const std::string &package) {
  auto success = open_and_run_file(L.get(), 1, SCRIPT_PATH_PREFIX + package + PACKAGE_ENTRYPOINT_FILE);
  if (!success) {
    return std::nullopt;
  }

  auto result = luabridge::LuaRef::fromStack(L.get(), -1);
  lua_pop(L.get(), 1);

  if (!result.isTable()) {
    std::cerr << "ScriptEnvironment: Package '" << package << "' did not return a table.\n";
    return std::nullopt;
  }

  return result;
}

inline std::optional<luabridge::LuaRef> ScriptEnvironment::get_package(const std::string &package) {
  if (package_cache.contains(package)) {
    return package_cache.at(package);
  }

  auto result = load_package(package);
  if (!result.has_value()) {
    return std::nullopt;
  }

  package_cache.insert_or_assign(package, result.value());

  return result;
}

template <typename T, typename F>
void ScriptEnvironment::register_component(
    entt::registry &ecs, const std::string &name, F &&fields_register_func
) {
  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("ECS")
      .addFunction(
          ("set" + name).c_str(),
          [&ecs](uint32_t entity_int, T component) {
            ecs.emplace_or_replace<T>(static_cast<entt::entity>(entity_int), component);
          }
      )
      .addFunction(
          ("has" + name).c_str(),
          [&ecs](uint32_t entity_int) { return ecs.all_of<T>(static_cast<entt::entity>(entity_int)); }
      )
      .addFunction(
          ("get" + name).c_str(),
          [&ecs](uint32_t entity_int) { return ecs.get<T>(static_cast<entt::entity>(entity_int)); }
      )
      .addFunction(
          ("remove" + name).c_str(),
          [&ecs](uint32_t entity_int) { ecs.remove<T>(static_cast<entt::entity>(entity_int)); }
      )
      .endNamespace();

  auto clazz = std::move(
      luabridge::getGlobalNamespace(L.get())
          .beginNamespace("Components")
          .beginClass<T>(name.c_str())
          .template addConstructor<void()>()
  );

  std::forward<F>(fields_register_func)(clazz);

  clazz.endClass().endNamespace();
}

template <typename F> void ScriptEnvironment::register_function(const std::string &name, F &&func) {
  luabridge::getGlobalNamespace(L.get())
      .beginNamespace(CPP_NAMESPACE.c_str())
      .addFunction(name.c_str(), std::forward<F>(func))
      .endNamespace();
}

inline lua_State &ScriptEnvironment::get_lua_state() {
  return *L;
}

template <typename Result, typename... Args>
Result ScriptEnvironment::call_global_function(const std::string &name, Args &&...args) {
  auto maybe_func = get_global_function(name);
  if (!maybe_func.has_value()) {
    return Result{};
  }

  auto result = maybe_func.value().call<Result>(std::forward<Args>(args)...);
  if (result.error()) {
    std::cerr << "ScriptEnvironment: failed to call global '" << name << "': " << result.message() << "\n";
    assert(false && "Failed to call global function.");
    return Result{};
  }

  return result.value();
}

template <typename Result, typename... Args>
Result ScriptEnvironment::call_function(const std::string &package, const std::string &name, Args &&...args) {
  auto maybe_func = get_function(package, name);
  if (!maybe_func.has_value()) {
    return Result{};
  }

  auto result = maybe_func.value().call<Result>(std::forward<Args>(args)...);
  if (result.error()) {
    std::cerr << "ScriptEnvironment: failed to call '" << name << "': " << result.error_cstr() << "\n";
    assert(false && "Failed to call function.");
    return Result{};
  }

  return result.value();
}

inline std::optional<luabridge::LuaRef>
get_function_from_table(const luabridge::LuaRef &table, const std::string &name) {
  if (!table.isTable()) {
    std::cerr << "ScriptEnvironment: tried to get'" << name << "' with a non table.\n";
    return std::nullopt;
  }

  std::vector<std::string> path;
  auto split_range = name | std::views::split('.');
  for (auto &&subrange : split_range) {
    path.emplace_back(subrange.begin(), subrange.end());
  }

  const std::string function_name = path.back();
  path.pop_back();

  auto name_space = table;

  for (const auto &element : path) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    name_space = name_space[element.c_str()];
    if (!name_space.isTable()) {
      std::cerr << "ScriptEnvironment: '" << name << "': '" << element << "' is not a table\n";
      return std::nullopt;
    }
  }

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
  auto func = name_space[function_name.c_str()];
  if (!func.isFunction()) {
    std::cerr << "ScriptEnvironment: '" << name << "' is not a function.\n";
    return std::nullopt;
  }

  return func;
}

inline std::optional<luabridge::LuaRef>
ScriptEnvironment::get_function(const std::string &package, const std::string &function) {
  const std::string cache_key = package + "::" + function;

  if (function_cache.contains(cache_key)) {
    return function_cache.at(cache_key);
  }

  auto maybe_package_ref = get_package(package);
  if (!maybe_package_ref.has_value()) {
    return std::nullopt;
  }

  auto maybe_func = get_function_from_table(maybe_package_ref.value(), function);
  if (!maybe_func.has_value()) {
    return std::nullopt;
  }
  auto func = maybe_func.value();

  function_cache.insert({cache_key, func});

  return func;
}

inline std::optional<luabridge::LuaRef> ScriptEnvironment::get_global_function(const std::string &function) {
  if (global_function_cache.contains(function)) {
    return global_function_cache.at(function);
  }

  auto table = luabridge::getGlobal(L.get(), "_G");

  auto maybe_func = get_function_from_table(table, function);
  if (!maybe_func.has_value()) {
    return std::nullopt;
  }
  auto func = maybe_func.value();

  global_function_cache.insert({function, func});

  return func;
}

inline void register_scene_components_to_script_env(SceneComponents args) {
  lua_State &L = args.scripts.get_lua_state();

  luaL_openlibs(&L);

  luabridge::getGlobalNamespace(&L)
      .beginNamespace("Game")
      .addProperty("WINDOW_WIDTH", &WINDOW_WIDTH)
      .addProperty("WINDOW_HEIGHT", &WINDOW_HEIGHT)
      .endNamespace()
      .beginNamespace("ECS")
      .addFunction("create", [&ecs = args.ecs]() -> uint32_t { return entt::to_integral(ecs.create()); })
      .endNamespace()
      .beginNamespace("AnimationStrips")
      .addFunction(
          "create",
          [&animation_strips = args.animation_strips]() -> uint8_t { return animation_strips.create(); }
      )
      .addFunction(
          "addFrame",
          [&animation_strips = args.animation_strips](uint8_t id, int x, int y) {
            animation_strips.add_frame(id, AnimationFrame{.x = x, .y = y});
          }
      )
      .endNamespace();
}

} // namespace framework
