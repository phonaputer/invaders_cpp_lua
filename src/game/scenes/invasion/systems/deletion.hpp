#pragma once

#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include <functional>

namespace systems {

class Deletion : public framework::System {
  private:
    std::reference_wrapper<framework::ScriptEnvironment> scripts;

  public:
    explicit Deletion(framework::ScriptEnvironment &scripts);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems