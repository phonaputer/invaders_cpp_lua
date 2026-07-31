#pragma once

#include "framework/scene.hpp"

namespace invasion {

class Scene : public framework::Scene {
  public:
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace invasion