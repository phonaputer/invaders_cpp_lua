#include "game/scenes/invasion/script_api.hpp"
#include "framework/constants.hpp"
#include "framework/event_broker.hpp"
#include "framework/scene.hpp"
#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include "game/scenes/invasion/components/game_over.hpp"
#include "game/scenes/invasion/components/hud.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/register_components.hpp"
#include "game/scenes/invasion/events/play_sound.hpp"
#include "game/scenes/invasion/events/stop_sound.hpp"
#include <algorithm>
#include <entt.hpp>
#include <lua.h>

#include <LuaBridge/LuaBridge.h>

namespace invasion {

void reset_score(entt::registry &ecs) {
  auto &hud = ecs.ctx().get<components::HUD>();
  hud.score = 0;
}

void increment_score(entt::registry &ecs, int amount) {
  auto &hud = ecs.ctx().get<components::HUD>();
  hud.score += amount;
  hud.high_score = std::max(hud.high_score, hud.score);
}

bool decrement_lives(entt::registry &ecs) {
  auto &hud = ecs.ctx().get<components::HUD>();

  if (hud.remaining_lives < 1) {
    return true;
  }

  hud.remaining_lives--;

  return false;
}

void set_lives(entt::registry &ecs, uint8_t lives) {
  auto &hud = ecs.ctx().get<components::HUD>();
  hud.remaining_lives = lives;
}

void pause(entt::registry &ecs) {
  if (!ecs.ctx().contains<components::Pause>()) {
    ecs.ctx().emplace<components::Pause>();
  }
}

void unpause(entt::registry &ecs) {
  if (ecs.ctx().contains<components::Pause>()) {
    ecs.ctx().erase<components::Pause>();
  }
}

void display_game_over(entt::registry &ecs) {
  if (!ecs.ctx().contains<components::GameOver>()) {
    ecs.ctx().emplace<components::GameOver>();
  }
}

void play_sound(framework::EventBroker &events, const std::string &audio_src) {
  events.push_back_draw<events::PlaySound>(events::PlaySound{.audio_src = audio_src});
}

void stop_sound(framework::EventBroker &events, const std::string &audio_src) {
  events.push_back_draw<events::StopSound>(events::StopSound{.audio_src = audio_src});
}

void register_cpp_api_to_script_env(framework::SceneInitializationContext &ctx) {
  components::register_components(ctx.ecs, &ctx.scripts.get_lua_state());
  components::register_damage_type_enum_to_script_env(ctx.scripts);

  ctx.scripts.register_function("Game", "incrementScore", [&ecs = ctx.ecs](lua_Number amount) {
    increment_score(ecs, static_cast<int>(amount));
  });
  ctx.scripts.register_function("Game", "resetScore", [&ecs = ctx.ecs]() { reset_score(ecs); });
  ctx.scripts.register_function("Game", "decrementLives", [&ecs = ctx.ecs]() {
    return decrement_lives(ecs);
  });
  ctx.scripts.register_function("Game", "setLives", [&ecs = ctx.ecs](lua_Number amount) {
    set_lives(ecs, static_cast<uint8_t>(amount));
  });
  ctx.scripts.register_function("Game", "pause", [&ecs = ctx.ecs]() { pause(ecs); });
  ctx.scripts.register_function("Game", "unpause", [&ecs = ctx.ecs]() { unpause(ecs); });
  ctx.scripts.register_function("Game", "displayGameOver", [&ecs = ctx.ecs]() { display_game_over(ecs); });
  ctx.scripts.register_function("Game", "playSound", [&events = ctx.events](const std::string &audio_src) {
    play_sound(events, audio_src);
  });
  ctx.scripts.register_function("Game", "stopSound", [&events = ctx.events](const std::string &audio_src) {
    stop_sound(events, audio_src);
  });

  luabridge::getGlobalNamespace(&ctx.scripts.get_lua_state())
      .beginNamespace("Game")
      .addProperty("WINDOW_WIDTH", &framework::WINDOW_WIDTH)
      .addProperty("WINDOW_HEIGHT", &framework::WINDOW_HEIGHT)
      .endNamespace();
}

} // namespace invasion