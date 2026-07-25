
#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/dummy.hpp"
#include <iostream>
#include <string>

void print_from_cpp(std::string str) {
  std::cout << str << "\n";
}

void hello_world() {
  const std::string source = R"(
  local s = "Hello! Now I will count to ten... 1"

  for i = 2, 10 do
      s = s .. " " .. i
  end

  test.print_from_cpp(s .. "!")
  )";

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luabridge::getGlobalNamespace(L).beginNamespace("test").addFunction("print_from_cpp", print_from_cpp).endNamespace();

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