#pragma once

#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include <functional>

namespace systems {

class PlayerAttack : public framework::System {
  private:
    std::reference_wrapper<framework::ScriptEnvironment> scripts;

  public:
    PlayerAttack(framework::ScriptEnvironment &scripts);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems