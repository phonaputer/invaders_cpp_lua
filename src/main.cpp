#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/caching_script_runner.hpp"
#include "game/scenes/dummy/components/position.hpp"
#include <iostream>
#include <memory>
#include <string>

struct LuaStateDeleter {
    void operator()(lua_State *L) const {
      lua_close(L);
    }
};

void print_from_cpp(components::Position pos) {
  std::cout << "Position(x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << ")\n";
}

void type_check(std::string type_id) {
  std::cout << type_id << '\n';
}

int main() {
  lua_State *raw_L = luaL_newstate();
  std::shared_ptr<lua_State> L{raw_L, LuaStateDeleter()};

  luaL_openlibs(L.get());

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("Components")
      .beginClass<components::Position>("Position")
      .addConstructor<void (*)(void)>()
      .addStaticFunction("GetTypeID", []() { return "This is a position"; })
      .addProperty("x", &components::Position::x, &components::Position::x)
      .addProperty("y", &components::Position::y, &components::Position::y)
      .addProperty("z", &components::Position::z, &components::Position::z)
      .endClass()
      .endNamespace();

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("test")
      .addFunction("print_from_cpp", print_from_cpp)
      .addFunction("type_check", type_check)
      .endNamespace();

  auto script_runner = std::make_unique<framework::CachingScriptRunner>(L);

  std::cout << "Run 1...\n";
  script_runner->run_script("hello_world");
  std::cout << "\nRun 2...\n";
  script_runner->run_script("hello_world");
  std::cout << "\nRun 3...\n";
  script_runner->run_script("hello_world");
}