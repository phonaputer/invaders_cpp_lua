#include "game/scenes/start_screen/rendering_system.hpp"
#include "framework/audio_player.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include "framework/scene_setter.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/scene.hpp"
#include "game/scenes/start_screen/rendering_system_component.hpp"
#include "game/util/text_renderer.hpp"
#include <memory>

namespace start_screen {

RenderingSystem::RenderingSystem(
    framework::SceneSetter &scene_setter, framework::AudioPlayer &audio_player, framework::Renderer &renderer
)
    : scene_setter{scene_setter},
      audio_player{audio_player},
      renderer{renderer},
      text_renderer{renderer} {
}

void RenderingSystem::execute(framework::ExecuteCtx &ctx) {
  auto state = ctx.ecs.ctx().get<RenderingSystemComponent>();

  if (state.game_start_engaged) {
    state.game_start_tick_counter++;
    if (state.game_start_tick_counter % GAME_START_BLINK_TICKS == 0) {
      state.game_start_blink = !state.game_start_blink;
    }
    ctx.ecs.ctx().insert_or_assign(state);

    if (state.game_start_tick_counter > GAME_START_TOTAL_TICKS) {
      scene_setter.set_scene(std::make_unique<invasion::Scene>());
    }
  }

  if (!state.game_start_engaged && ctx.player_input.is_initiated(framework::PlayerInput::FIRE)) {
    state.game_start_engaged = true;
    audio_player.play_sound("menu_select.wav");
    ctx.ecs.ctx().insert_or_assign(state);
  }

  text_renderer.render_text_centered(60, "personal space invaders");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = "space_invaders.png",
          .src_x = 64,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 85,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(90, " - 10 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = "space_invaders.png",
          .src_x = 16,
          .src_y = 16,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 100,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(105, " - 20 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = "space_invaders.png",
          .src_x = 16,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 115,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(120, " - 30 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = "space_invaders.png",
          .src_x = 96,
          .src_y = 0,
          .src_width = 16,
          .src_height = 16,
          .dst_x = 60,
          .dst_y = 130,
          .dst_width = 16,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(135, " - 40 points");

  renderer.draw_image(
      framework::DrawImageParams{
          .src_id = "space_invaders.png",
          .src_x = 96,
          .src_y = 64,
          .src_width = 24,
          .src_height = 16,
          .dst_x = 52,
          .dst_y = 145,
          .dst_width = 24,
          .dst_height = 16
      }
  );
  text_renderer.render_text_centered(150, " -  ? points");

  text_renderer.render_text_centered(180, "<a> and <d> to move");
  text_renderer.render_text_centered(190, "<space> to shoot");

  if (!state.game_start_engaged || (state.game_start_engaged && state.game_start_blink)) {
    text_renderer.render_text_centered(220, "press <space> to begin");
  }
}

} // namespace start_screen