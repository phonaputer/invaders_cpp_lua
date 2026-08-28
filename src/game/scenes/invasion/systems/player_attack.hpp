#pragma once

#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/infra/weapon_registry.hpp"
#include <functional>

namespace systems {

class PlayerAttack : public framework::System {
  private:
    std::reference_wrapper<framework::ScriptEnvironment> scripts;
    std::reference_wrapper<infra::CallbackGetter> callbacks;
    std::reference_wrapper<infra::WeaponGetter> weapons;

  public:
    PlayerAttack(
        framework::ScriptEnvironment &scripts, infra::CallbackGetter &callbacks, infra::WeaponGetter &weapons
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems