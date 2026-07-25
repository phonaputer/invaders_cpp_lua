
#include <luacode.h>
#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/dummy.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void print_from_cpp(std::string str) {
  std::cout << str << "\n";
}

void hello_world() {
  std::ifstream script("./scripts/hello_world.luau");
  std::stringstream buffer;
  buffer << script.rdbuf();
  const std::string source = buffer.str();

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