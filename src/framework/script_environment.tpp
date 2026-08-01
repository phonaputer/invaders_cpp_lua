#pragma once

#include <entt.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <luacode.h>
#include <lualib.h>
#include <memory>
#include <sstream>
#include <string>

#include <LuaBridge/LuaBridge.h>

namespace framework {

constexpr std::string CPP_NAMESPACE = "Host";
constexpr std::string SCRIPT_PATH_PREFIX = "./scripts/";
constexpr std::string SCRIPT_PATH_SUFFIX = ".luau";

struct FreeDeleter {
    void operator()(void *ptr) const {
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory, cppcoreguidelines-no-malloc, hicpp-no-malloc)
      std::free(ptr);
    }
};

inline void ScriptEnvironment::open_and_run_file(const std::string &path) {
  std::ifstream script_file(path);
  if (!script_file.is_open()) {
    std::cerr << "ScriptEnvironment: Failed to open script file: " << path << "\n";
    assert(script_file.is_open() && "Failed to open script file.");
    return;
  }

  std::stringstream script_buffer;
  script_buffer << script_file.rdbuf();
  assert(script_buffer.str().length() > 0 && "Loaded empty script.");

  const std::string source = script_buffer.str();

  size_t bytecode_size = 0;
  const std::unique_ptr<char, FreeDeleter> bytecode{
      luau_compile(source.c_str(), source.length(), nullptr, &bytecode_size), FreeDeleter()
  };

  auto result = luau_load(L.get(), "HelloWorld", bytecode.get(), bytecode_size, 0);
  if (result != LUA_OK) {
    std::cerr << "ScriptEnvironment: Failed to load bytecode '" << path << "': " << lua_tostring(L.get(), -1)
              << "\n";
    assert(false && "Failed to load Luau bytecode.");
    return;
  }

  result = lua_pcall(L.get(), 0, 0, 0);
  if (result != LUA_OK) {
    std::cerr << "ScriptEnvironment: Failed to execute script '" << path << "': " << lua_tostring(L.get(), -1)
              << "\n";
    assert(false && "Failed to run Luau script.");
    return;
  }
}

inline ScriptEnvironment::ScriptEnvironment(entt::registry &ecs, AnimationStripRegistry &animation_strips)
    : ecs{ecs},
      L{luaL_newstate(), LuaStateDeleter()} {
  luaL_openlibs(L.get());

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("ECS")
      .addFunction("create", [&ecs]() -> uint32_t { return entt::to_integral(ecs.create()); })
      .endNamespace()
      .beginNamespace("AnimationStrips")
      .addFunction("create", [&animation_strips]() -> uint8_t { return animation_strips.create(); })
      .addFunction(
          "addFrame",
          [&animation_strips](uint8_t id, int x, int y) {
            animation_strips.add_frame(id, AnimationFrame{.x = x, .y = y});
          }
      )
      .endNamespace();
}

inline void ScriptEnvironment::exec_script_file(const std::string &path) {
  open_and_run_file(SCRIPT_PATH_PREFIX + path + SCRIPT_PATH_SUFFIX);
}

inline void ScriptEnvironment::exec_all_script_files_in_dir(const std::string &path) {
  try {
    for (const auto &entry : std::filesystem::directory_iterator(SCRIPT_PATH_PREFIX + path)) {
      open_and_run_file(entry.path().string());
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cout << "ScriptEnvironment: Failed to open directory '" << path << "': " << e.what() << "\n";
    assert(false && "Failed to open directory");
  }
}

template <typename T, typename F>
void ScriptEnvironment::register_component(const std::string &name, F &&fields_register_func) {
  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("ECS")
      .addFunction(
          ("set" + name).c_str(),
          [this](uint32_t entity_int, T component) {
            ecs.get().emplace_or_replace<T>(static_cast<entt::entity>(entity_int), component);
          }
      )
      .addFunction(
          ("has" + name).c_str(),
          [this](uint32_t entity_int) { return ecs.get().all_of<T>(static_cast<entt::entity>(entity_int)); }
      )
      .addFunction(
          ("get" + name).c_str(),
          [this](uint32_t entity_int) { return ecs.get().get<T>(static_cast<entt::entity>(entity_int)); }
      )
      .addFunction(
          ("remove" + name).c_str(),
          [this](uint32_t entity_int) { ecs.get().remove<T>(static_cast<entt::entity>(entity_int)); }
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

template <typename Result, typename... Args>
Result ScriptEnvironment::call_function(const std::string &name, Args &&...args) {
  auto func = luabridge::getGlobal(L.get(), name.c_str());

  if (!func.isFunction()) {
    std::cerr << "ScriptEnvironment: '" << name << "' is not a function\n";
    assert(false && "Tried to call a non-function type.");
    return Result{};
  }

  auto result = func.call<Result>(std::forward<Args>(args)...);
  if (result.error()) {
    std::cerr << "ScriptEnvironment: failed to call '" << name << "': " << result.message() << "\n";
    assert(false && "Failed to call function.");
    return Result{};
  }

  return result.value();
}

} // namespace framework
