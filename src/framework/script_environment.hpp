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

struct LuaStateDeleter {
    void operator()(lua_State *L) const {
      lua_close(L);
    }
};

struct FreeDeleter {
    void operator()(void *ptr) const {
      std::free(ptr);
    }
};

class ScriptEnvironment {
  private:
    entt::registry &ecs;
    std::unique_ptr<lua_State, LuaStateDeleter> L;

    void exec_single_script_file(std::string path) {
      std::ifstream script_file(path + SCRIPT_PATH_SUFFIX);
      assert(script_file.is_open() && "Failed to open script file.");

      std::stringstream script_buffer;
      script_buffer << script_file.rdbuf();
      assert(script_buffer.str().length() > 0 && "Loaded empty script.");

      const std::string source = script_buffer.str();

      size_t bytecode_size = 0;
      std::unique_ptr<char, FreeDeleter> bytecode{
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
    };

  public:
    ScriptEnvironment(entt::registry &ecs)
        : ecs{ecs},
          L{luaL_newstate(), LuaStateDeleter()} {
      luaL_openlibs(L.get());

      luabridge::getGlobalNamespace(L.get())
          .beginNamespace("ECS")
          .addFunction("create", [&ecs]() { return entt::to_integral(ecs.create()); })
          .endNamespace();
    };

    void exec_script_file(std::string path) {
      exec_single_script_file(SCRIPT_PATH_PREFIX + path);
    };

    void exec_all_script_files_in_dir(std::string path) {
      for (const auto &entry : std::filesystem::directory_iterator(SCRIPT_PATH_PREFIX + path)) {
        exec_script_file(entry.path().string());
      }
    };

    template <typename T, typename F> void register_component(std::string name, F &&fields_register_func) {
      luabridge::getGlobalNamespace(L.get())
          .beginNamespace("ECS")
          .addFunction(
              ("set" + name).c_str(),
              [this](uint32_t e_int, T t) { ecs.emplace_or_replace<T>(static_cast<entt::entity>(e_int), t); }
          )
          .addFunction(
              ("has" + name).c_str(), [this](uint32_t e_int) { return ecs.all_of<T>(static_cast<entt::entity>(e_int)); }
          )
          .addFunction(
              ("get" + name).c_str(), [this](uint32_t e_int) { return ecs.get<T>(static_cast<entt::entity>(e_int)); }
          )
          .addFunction(
              ("remove" + name).c_str(), [this](uint32_t e_int) { ecs.remove<T>(static_cast<entt::entity>(e_int)); }
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

    template <typename F> void register_function(const std::string &name, F &&func) {
      luabridge::getGlobalNamespace(L.get())
          .beginNamespace(CPP_NAMESPACE.c_str())
          .addFunction(name.c_str(), std::forward<F>(func))
          .endNamespace();
    }

    template <typename Result = void, typename... Args> Result call_function(const std::string &name, Args &&...args) {
      auto func = luabridge::getGlobal(L.get(), name.c_str());

      if (!func.isFunction()) {
        std::cerr << "ScriptEnvironment: '" << name << "' is not a function\n";
        assert(false && "Tried to call a non-function type.");
      }

      auto result = func.call<Result>(std::forward<Args>(args)...);
      if (result.error()) {
        std::cerr << "ScriptEnvironment: failed to call '" << name << "': " << result.error_cstr() << "\n";
        assert(false && "Failed to call function.");
      }

      return result.value();
    }
};

} // namespace framework
