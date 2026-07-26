#define SDL_MAIN_USE_CALLBACKS

#include "framework/game.hpp"
#include "framework/player_input_manager.hpp"
#include "game/scenes/dummy/scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  framework::Game *game = nullptr;
  try {
    game = new framework::Game();
    *appstate = game;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

  auto scene = std::make_unique<dummy::Scene>();
  game->set_scene(std::move(scene));

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, [[maybe_unused]] SDL_Event *event) {
  auto game = (framework::Game *)appstate;

  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_player_input_manager().engage(framework::PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_player_input_manager().engage(framework::PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_player_input_manager().engage(framework::PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_player_input_manager().engage(framework::PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_player_input_manager().engage(framework::PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_player_input_manager().engage(framework::PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;

    case SDL_EVENT_KEY_UP:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_player_input_manager().disengage(framework::PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_player_input_manager().disengage(framework::PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_player_input_manager().disengage(framework::PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_player_input_manager().disengage(framework::PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_player_input_manager().disengage(framework::PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_player_input_manager().disengage(framework::PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto game = (framework::Game *)appstate;

  game->update();
  game->draw();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result) {
  auto game = (framework::Game *)appstate;

  delete game;

  SDL_Log("Quitting...");
}