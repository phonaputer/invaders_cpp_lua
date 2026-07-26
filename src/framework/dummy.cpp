
#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/caching_script_runner.hpp"
#include "framework/dummy.hpp"
#include <iostream>
#include <memory>
#include <string>

struct LuaStateDeleter {
    void operator()(lua_State *L) const {
      lua_close(L);
    }
};

void print_from_cpp(std::string str) {
  std::cout << str << "\n";
}

void hello_world() {
  lua_State *raw_L = luaL_newstate();
  std::shared_ptr<lua_State> L{raw_L, LuaStateDeleter()};

  luaL_openlibs(L.get());

  luabridge::getGlobalNamespace(L.get())
      .beginNamespace("test")
      .addFunction("print_from_cpp", print_from_cpp)
      .endNamespace();

  auto script_runner = std::make_unique<framework::CachingScriptRunner>(L);

  std::cout << "Run 1...\n";
  script_runner->run_script("hello_world");
  std::cout << "\nRun 2...\n";
  script_runner->run_script("hello_world");
  std::cout << "\nRun 3...\n";
  script_runner->run_script("hello_world");
}