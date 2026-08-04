#include "framework/game.hpp"
#include "framework/constants.hpp"
#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/scene.hpp"
#include "framework/script_environment.hpp"
#include "framework/sdl_asset_manager.hpp"
#include "framework/sdl_renderer.hpp"
#include "framework/system.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdint>
#include <entt.hpp>
#include <format>
#include <memory>
#include <optional>
#include <stdexcept>

namespace framework {

const uint64_t TARGET_FPS = 60;
const uint64_t MS_PER_UPDATE = 1000 / TARGET_FPS;

const int ACTUAL_WINDOW_WIDTH = 448;
const int ACTUAL_WINDOW_HEIGHT = 576;

struct SDLMixerDeleter {
    void operator()(MIX_Mixer *mixer_p) const {
      MIX_DestroyMixer(mixer_p);
    }
};

Game::Game() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow(
      "Invaders... from space!!!",
      ACTUAL_WINDOW_WIDTH,
      ACTUAL_WINDOW_HEIGHT,
      SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
  );
  if (window == nullptr) {
    throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    throw std::runtime_error(std::format("Couldn't create renderer: {}", SDL_GetError()));
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SDL_ShowWindow(window);

  if (!MIX_Init()) {
    throw std::runtime_error(std::format("Couldn't initialize sound mixer: {}", SDL_GetError()));
  }

  auto *mixerP = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (mixerP == nullptr) {
    throw std::runtime_error(std::format("Couldn't initialize mixer device: {}", SDL_GetError()));
  }
  mixer = std::shared_ptr<MIX_Mixer>(mixerP, SDLMixerDeleter());

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;

  player_input_manager = std::make_unique<PlayerInputManager>();
  asset_manager = std::make_shared<SDLAssetManager>(renderer, mixer);
  renderer_wrapper = std::make_unique<SDLRenderer>(renderer, asset_manager);
}

void Game::update() {
  apply_new_scene_if_any();

  if (!have_active_scene) {
    return;
  }

  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  ExecuteCtx ctx = {
      .ecs = ecs,
      .events = event_broker,
      .player_input = *player_input_manager,
  };

  event_broker.clear_all_draw();

  while (unprocessed_ms > MS_PER_UPDATE) {
    player_input_manager->update();
    event_broker.clear_all();

    for (const auto &system : update_systems) {
      system->execute(ctx);
    }

    unprocessed_ms -= MS_PER_UPDATE;
  }

  // Doing this should save some CPU by preventing SDL from executing this
  // function as fast as possible.
  // A cursory test seemed to indicate this cuts CPU usage by about 50%.
  const uint64_t frameTime = SDL_GetTicks() - now_ms;
  if (MS_PER_UPDATE > frameTime) {
    SDL_Delay(MS_PER_UPDATE - frameTime);
  }
}

void Game::draw() {
  if (!have_active_scene) {
    return;
  }

  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  ExecuteCtx ctx = {
      .ecs = ecs,
      .events = event_broker,
      .player_input = *player_input_manager,
  };

  for (const auto &system : draw_systems) {
    system->execute(ctx);
  }

  SDL_RenderPresent(renderer);
}

void Game::set_scene(std::unique_ptr<Scene> scene) {
  new_scene = std::move(scene);
};

PlayerInputManager &Game::get_player_input_manager() {
  return *player_input_manager;
}

void Game::add_update_system(std::unique_ptr<System> system) {
  update_systems.push_back(std::move(system));
}

void Game::add_draw_system(std::unique_ptr<System> system) {
  draw_systems.push_back(std::move(system));
}

void Game::apply_new_scene_if_any() {
  if (!new_scene.has_value()) {
    return;
  }

  ecs.clear();
  asset_manager->clear_all();
  update_systems.clear();
  draw_systems.clear();
  animation_strips.clear();
  scripts = std::make_unique<ScriptEnvironment>(ecs, animation_strips);

  new_scene.value()->initialize(
      SceneInitializationContext{
          .assets = *asset_manager,
          .systems = *this,
          .ecs = ecs,
          .renderer = *renderer_wrapper,
          .audio_player = *asset_manager,
          .scene_setter = *this,
          .scripts = *scripts,
          .animation_strips = animation_strips,
      }
  );

  new_scene = std::nullopt;
  have_active_scene = true;
}

} // namespace framework