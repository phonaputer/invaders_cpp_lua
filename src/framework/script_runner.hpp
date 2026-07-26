#pragma once

#include <string>

namespace framework {

class ScriptRunner {
  public:
    virtual ~ScriptRunner() = default;
    ScriptRunner(const ScriptRunner &) = delete;
    ScriptRunner &operator=(const ScriptRunner &) = delete;
    ScriptRunner(ScriptRunner &&) = delete;
    ScriptRunner &operator=(ScriptRunner &&) = delete;

    virtual void run_script(std::string path) = 0;
};

} // namespace framework