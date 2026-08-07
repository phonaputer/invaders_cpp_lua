#pragma once

#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <functional>

namespace systems {

class CollisionCallback : public framework::System {
  private:
    std::reference_wrapper<framework::ScriptEnvironment> scripts;
    std::reference_wrapper<infra::CallbackGetter> callbacks;

  public:
    CollisionCallback(framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems