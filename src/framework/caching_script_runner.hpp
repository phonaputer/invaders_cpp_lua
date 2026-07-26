#pragma once

#include "framework/script_runner.hpp"
#include <luacode.h>
#include <lualib.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace framework {

struct CFreeDeleter {
    void operator()(void *ptr) const {
      std::free(ptr);
    }
};

struct LuauBytecode {
    std::unique_ptr<char, CFreeDeleter> bytecode;
    size_t size;
};

// TODO - Support hot loading when a script changes.
class CachingScriptRunner : public ScriptRunner {
  private:
    std::shared_ptr<lua_State> L;
    std::unordered_map<std::string, LuauBytecode> bytecode_cache;

    void load_script_into_cache(const std::string &path);

  public:
    CachingScriptRunner(std::shared_ptr<lua_State> L);
    void run_script(const std::string &path) override;
    void clear_cache();
};

} // namespace framework