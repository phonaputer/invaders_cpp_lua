#pragma once

#include "framework/scene.hpp"
#include <memory>

namespace framework {

class Scene;

class SceneSetter {
  public:
    virtual ~SceneSetter() = default;
    virtual void set_scene(std::unique_ptr<Scene> scene) = 0;
};

} // namespace framework