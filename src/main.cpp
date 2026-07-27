#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/caching_script_runner.hpp"
#include <entt.hpp>
#include <iostream>
#include <memory>
#include <string>

struct Position {
    float x;
    float y;
    float z;

    static constexpr std::string TYPE_ID = "Position";
};

void print_position(Position pos) {
  std::cout << "Position(x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << ")\n";
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

  luabridge::getGlobalNamespace(&L)
      .beginNamespace("ECS")
      .addFunction(
          "setPosition",
          [&ecs](uint32_t e_int, Position p) { ecs.emplace_or_replace<Position>(static_cast<entt::entity>(e_int), p); }
      )
      .addFunction(
          "hasPosition", [&ecs](uint32_t e_int) { return ecs.all_of<Position>(static_cast<entt::entity>(e_int)); }
      )
      .addFunction(
          "getPosition", [&ecs](uint32_t e_int) { return ecs.get<Position>(static_cast<entt::entity>(e_int)); }
      )
      .addFunction("removePosition", [&ecs](uint32_t e_int) { ecs.remove<Position>(static_cast<entt::entity>(e_int)); })
      .endNamespace();
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