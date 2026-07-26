#pragma once

#include "framework/asset_loader.hpp"
#include "framework/audio_player.hpp"
#include "framework/renderer.hpp"
#include "framework/scene_setter.hpp"
#include "framework/system_registry.hpp"
#include <entt.hpp>

namespace framework {

class SceneSetter;

struct SceneInitializationContext {
    AssetLoader &assets;
    SystemRegistry &systems;
    entt::registry &ecs;
    Renderer &renderer;
    AudioPlayer &audio_player;
    SceneSetter &scene_setter;
};

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void initialize(SceneInitializationContext ctx) = 0;
};

} // namespace framework