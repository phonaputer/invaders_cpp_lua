#include "game/scenes/invasion/systems/hud_rendering.hpp"
#include "framework/constants.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/game_over.hpp"
#include "game/scenes/invasion/components/hud.hpp"
#include "game/scenes/invasion/constants.hpp"
#include <format>

namespace systems {

HUDRendering::HUDRendering(framework::Renderer &renderer)
    : renderer{renderer},
      text_renderer{renderer} {
}

void HUDRendering::execute(framework::ExecuteCtx &ctx) {
  renderer.draw_line(
      framework::DrawLineParams{
          .start_x = 0,
          .start_y = invasion::GROUND_HEIGHT,
          .end_x = framework::WINDOW_WIDTH,
          .end_y = invasion::GROUND_HEIGHT,
          .r = 0,
          .g = 255,
          .b = 0,
      }
  );

  auto hud = ctx.ecs.ctx().get<components::HUD>();

  text_renderer.render_text(6, 6, std::format("score:{}", hud.score));

  const int high_score_x_position = (framework::WINDOW_WIDTH / 2) - 31;
  text_renderer.render_text(high_score_x_position, 6, std::format("hi-score:{}", hud.high_score));

  text_renderer.render_text(framework::WINDOW_WIDTH - 48, 6, std::format("lives:{}", hud.remaining_lives));

  if (ctx.ecs.ctx().contains<components::GameOver>()) {
    const int game_over_x_position = (framework::WINDOW_WIDTH / 2) - 28;
    const int game_over_y_position = framework::WINDOW_HEIGHT / 2;
    text_renderer.render_text(game_over_x_position, game_over_y_position, "game over");
  }
}

} // namespace systems