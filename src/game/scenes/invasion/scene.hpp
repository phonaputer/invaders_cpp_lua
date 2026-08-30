#pragma once

#include "framework/scene.hpp"
#include "game/scenes/invasion/infra/animation_strip_registry.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/infra/weapon_registry.hpp"

namespace invasion {

class Scene : public framework::Scene {
  private:
    infra::AnimationStripRegistry animation_strip_registry;
    infra::CallbackRegistry callback_registry;
    infra::WeaponRegistry weapon_registry;

  public:
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace invasion