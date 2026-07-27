#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/caching_script_runner.hpp"
#include <entt.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Position {
    float x;
    float y;
    float z;

    static constexpr std::string TYPE_ID = "Position";
};

void print_position(Position pos) {
  std::cout << "Position(x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << ")\n";
}

int varargs_function_raw_style(lua_State *L) {
  int numArgs = lua_gettop(L);

  for (int i = 1; i <= numArgs; i++) {
    luabridge::LuaRef ref = luabridge::LuaRef::fromStack(L, i);

    if (!ref.isString()) {
      std::cout << "Where is my RAW STYLE strang, mang\n";
    } else {
      std::cout << ref.cast<std::string>().value() << ", ";
    }
  }

  std::cout << "\n";

  return 0;
}

void varargs_function(luabridge::LuaRef ref) {
  if (!ref.isTable()) {
    std::cout << "I'm tabling this motion\n";
    return;
  }

  for (int i = 1; i <= ref.length(); i++) {
    auto elem = ref[i];

    if (!elem.isString()) {
      std::cout << "Where is my strang, mang\n";
    } else {
      std::cout << elem.cast<std::string>().value() << ", ";
    }
  }

  std::cout << "\n";
}

template <typename T> void register_crud_functions_for_component(lua_State &L, entt::registry &ecs, std::string name) {
  // clang-format off
    luabridge::getGlobalNamespace(&L)
      .beginNamespace("ECS")
      .addFunction(
          ("set" + name).c_str(),
          [&ecs](uint32_t e_int, T t) { ecs.emplace_or_replace<T>(static_cast<entt::entity>(e_int), t); }
      )
      .addFunction(
          ("has" + name).c_str(), 
          [&ecs](uint32_t e_int) { return ecs.all_of<T>(static_cast<entt::entity>(e_int)); }
      )
      .addFunction(
          ("get" + name).c_str(), 
          [&ecs](uint32_t e_int) { return ecs.get<T>(static_cast<entt::entity>(e_int)); }
      )
      .addFunction(
          ("remove" + name).c_str(), 
          [&ecs](uint32_t e_int) { ecs.remove<T>(static_cast<entt::entity>(e_int)); }
      )
      .endNamespace();
  // clang-format on
}

void position_register_to_lua(lua_State &L, entt::registry &ecs) {
  luabridge::getGlobalNamespace(&L)
      .beginNamespace("Components")
      .beginClass<Position>("Position")
      .addConstructor<void (*)(void)>()
      .addStaticFunction("GetTypeID", []() { return Position::TYPE_ID; })
      .addProperty("x", &Position::x, &Position::x)
      .addProperty("y", &Position::y, &Position::y)
      .addProperty("z", &Position::z, &Position::z)
      .endClass()
      .endNamespace();

  register_crud_functions_for_component<Position>(L, ecs, "Position");
}

void run_callback(luabridge::LuaRef callback) {
  if (!callback.isFunction()) {
    std::cout << "what have you done, son?\n";
    return;
  }

  callback("good golly miss molly");
}

struct LuaStateDeleter {
    void operator()(lua_State *L) const {
      lua_close(L);
    }
};

int main() {
  entt::registry ecs;

  lua_State *raw_L = luaL_newstate();
  std::shared_ptr<lua_State> L{raw_L, LuaStateDeleter()};

  luaL_openlibs(L.get());

  position_register_to_lua(*L, ecs);

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("ECS")
      .addFunction(
          "create",
          [&ecs]() {
            auto e = ecs.create();
            return entt::to_integral(e);
          }
      )
      .endNamespace();

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("test")
      .addFunction("printPosition", print_position)
      .addFunction("runCallback", run_callback)
      .addFunction("printAll", varargs_function)
      .addFunction("printAllRAW", varargs_function_raw_style)
      .endNamespace();

  auto script_runner = std::make_unique<framework::CachingScriptRunner>(L);

  std::cout << "---Run 1---\n";
  script_runner->run_script("hello_world");
  std::cout << "---Run 1---\n\n---Run 2---\n";
  script_runner->run_script("hello_world");
  std::cout << "---Run 2---\n\n---Run 3---\n";
  script_runner->run_script("hello_world");
  std::cout << "---Run 3---\n";

  std::cout << "\n\n=====Finished Lua Runs=====\n\n\n";

  auto view = ecs.view<Position>();

  for (auto [e, position] : view.each()) {
    std::cout << "---Entity " << entt::to_integral(e) << "---\n";
    print_position(position);
    std::cout << "---Entity " << entt::to_integral(e) << "---\n\n";
  }
}