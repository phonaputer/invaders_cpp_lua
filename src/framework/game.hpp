#pragma once

#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/scene.hpp"
#include "framework/scene_setter.hpp"
#include "framework/script_environment.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include "framework/system.hpp"
#include "framework/system_registry.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <entt.hpp>
#include <memory>
#include <optional>

namespace framework {

// TODO - Maybe SDL stuff could get moved out to another class to make this smaller?
// It also would be nice to review the interface this provides to scenes to see if it could be cleaner.
class Game : public SystemRegistry, public SceneSetter {
  private:
    // Window and Renderer are owned and cleaned up by SDL.
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::shared_ptr<MIX_Mixer> mixer;

    std::unique_ptr<Renderer> renderer_wrapper;
    std::unique_ptr<PlayerInputManager> player_input_manager;
    std::shared_ptr<SDLAssetManager> asset_manager;
    EventBroker event_broker;
    entt::registry ecs;
    std::unique_ptr<ScriptEnvironment> scripts;

    std::vector<std::unique_ptr<System>> update_systems;
    std::vector<std::unique_ptr<System>> draw_systems;

    bool have_active_scene = false;
    std::optional<std::unique_ptr<Scene>> new_scene;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

    void apply_new_scene_if_any();

  public:
    Game();
    void update();
    void draw();
    PlayerInputManager &get_player_input_manager();

    void set_scene(std::unique_ptr<Scene> scene) override;

    void add_update_system(std::unique_ptr<System> system) override;
    void add_draw_system(std::unique_ptr<System> system) override;
};

} // namespace framework