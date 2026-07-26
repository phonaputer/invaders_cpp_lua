#pragma once

#include "framework/scene.hpp"

namespace dummy {

class Scene : public framework::Scene {
  public:
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace dummy