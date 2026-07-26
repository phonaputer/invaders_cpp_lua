#include "framework/caching_script_runner.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <lua.h>
#include <luacode.h>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace framework {

constexpr std::string SCRIPT_PATH_PREFIX = "./scripts/";
constexpr std::string SCRIPT_PATH_SUFFIX = ".luau";

CachingScriptRunner::CachingScriptRunner(std::shared_ptr<lua_State> L)
    : L{std::move(L)} {
}

void CachingScriptRunner::run_script(const std::string &path) {
  if (!bytecode_cache.contains(path)) {
    load_script_into_cache(path);
  }

  assert(bytecode_cache.contains(path) && "Failed to load Luau script into cache.");

  const auto &bytecode = bytecode_cache.at(path);

  auto result = luau_load(L.get(), "HelloWorld", bytecode.bytecode.get(), bytecode.size, 0);
  if (result != LUA_OK) {
    std::cerr << "Error loading Luau bytecode (" << path << "): " << lua_tostring(L.get(), -1) << "\n";
    assert(false && "Failed to load Luau bytecode.");
    return;
  }

  result = lua_pcall(L.get(), 0, 0, 0);
  if (result != LUA_OK) {
    std::cerr << "Error executing Luau (" << path << "): " << lua_tostring(L.get(), -1) << "\n";
    assert(false && "Failed to run Luau script.");
    return;
  }
}

void CachingScriptRunner::clear_cache() {
  bytecode_cache.clear();
}

void CachingScriptRunner::load_script_into_cache(const std::string &path) {
  std::ifstream script_file(SCRIPT_PATH_PREFIX + path + SCRIPT_PATH_SUFFIX);
  assert(script_file.is_open() && "Failed to open script file.");

  std::stringstream script_buffer;
  script_buffer << script_file.rdbuf();
  assert(script_buffer.str().length() > 0 && "Loaded empty script.");

  const std::string source = script_buffer.str();

  size_t bytecode_size = 0;
  auto *bytecode_raw_ptr = luau_compile(source.c_str(), source.length(), nullptr, &bytecode_size);
  std::unique_ptr<char, CFreeDeleter> bytecode{bytecode_raw_ptr, CFreeDeleter()};

  bytecode_cache.try_emplace(path, std::move(bytecode), bytecode_size);
}

} // namespace framework