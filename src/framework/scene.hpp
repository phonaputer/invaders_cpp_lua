#pragma once

#include "framework/animation_strip_registry.hpp"
#include "framework/asset_loader.hpp"
#include "framework/audio_player.hpp"
#include "framework/event_broker.hpp"
#include "framework/renderer.hpp"
#include "framework/script_environment.hpp"
#include "framework/system_registry.hpp"
#include <entt.hpp>
#include <lualib.h>

namespace framework {

class SceneSetter;

// Intentionally using references here to simplify the API.
// And anyways this is just a parameter object to clean up the initialize function signature.
//
// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct SceneInitializationContext {
    AssetLoader &assets;
    SystemRegistry &systems;
    entt::registry &ecs;
    Renderer &renderer;
    AudioPlayer &audio_player;
    SceneSetter &scene_setter;
    ScriptEnvironment &scripts;
    AnimationStripRegistry &animation_strips;
    framework::EventBroker &events;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

class Scene {
  public:
    Scene() = default;
    virtual ~Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(Scene &&) = delete;

    virtual void initialize(SceneInitializationContext ctx) = 0;
};

} // namespace framework