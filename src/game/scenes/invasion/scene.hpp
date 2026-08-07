#pragma once

#include "framework/scene.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace invasion {

class Scene : public framework::Scene {
  private:
    infra::CallbackRegistry callback_registry;

  public:
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace invasion