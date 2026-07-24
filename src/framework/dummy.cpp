#include "framework/dummy.hpp"

#include <iostream>
#include <luacode.h>
#include <lualib.h>
#include <string>

void hello_world() {
  const std::string source = R"(print("Hello from Lua!"))";

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  size_t bytecodeSize = 0;
  char *bytecode = luau_compile(source.c_str(), source.length(), NULL, &bytecodeSize);

  if (luau_load(L, "HelloWorld", bytecode, bytecodeSize, 0) != LUA_OK) {
    std::cerr << "Error loading string: " << lua_tostring(L, -1) << std::endl;
    lua_close(L);
    free(bytecode);
    return;
  }

  free(bytecode);

  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    std::cerr << "Error running code: " << lua_tostring(L, -1) << std::endl;
  }

  lua_close(L);
}