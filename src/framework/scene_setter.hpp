#pragma once

#include <memory>

namespace framework {

class Scene;

class SceneSetter {
  public:
    SceneSetter() = default;
    virtual ~SceneSetter() = default;
    SceneSetter(const SceneSetter &) = delete;
    SceneSetter &operator=(const SceneSetter &) = delete;
    SceneSetter(SceneSetter &&) = delete;
    SceneSetter &operator=(SceneSetter &&) = delete;

    virtual void set_scene(std::unique_ptr<Scene> scene) = 0;
};

} // namespace framework