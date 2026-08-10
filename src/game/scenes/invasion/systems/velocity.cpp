#include "game/scenes/invasion/systems/velocity.hpp"
#include "framework/constants.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/pause.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/velocity.hpp"

namespace systems {

constexpr float OFFSCREEN_BOUNDARY = 5.0F;

void Velocity::execute(framework::ExecuteCtx &ctx) {
  if (ctx.ecs.ctx().contains<components::Pause>()) {
    return;
  }

  auto view = ctx.ecs.view<components::Position, components::Velocity>();
  view.use<components::Position>();

  for (auto [entity, position, velocity] : view.each()) {
    position.x += velocity.x;
    position.y += velocity.y;
  }

  auto deletion_view = ctx.ecs.view<components::Position, components::Velocity>();

  for (auto [entity, position, velocity] : deletion_view.each()) {
    if (position.x > framework::WINDOW_WIDTH + OFFSCREEN_BOUNDARY
        || position.x + position.w < 0 - OFFSCREEN_BOUNDARY
        || position.y > framework::WINDOW_HEIGHT + OFFSCREEN_BOUNDARY
        || position.y + position.h < 0 - OFFSCREEN_BOUNDARY) {
      ctx.ecs.emplace_or_replace<components::ToBeDeleted>(entity);
    }
  }
}

} // namespace systems