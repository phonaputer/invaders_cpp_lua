#include "game/scenes/invasion/systems/debug_hitbox_rendering.hpp"

#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/collision_active.hpp"
#include "game/scenes/invasion/components/collision_passive.hpp"
#include "game/scenes/invasion/components/position.hpp"

namespace systems {

DebugHitboxRendering::DebugHitboxRendering(framework::Renderer &renderer)
    : renderer{renderer} {
}

void DebugHitboxRendering::execute(framework::ExecuteCtx &ctx) {
  auto view_passive = ctx.ecs.view<components::Position, components::CollisionPassive>();
  for (auto [entity, position, collision] : view_passive.each()) {
    renderer.draw_rect(
        framework::DrawRectParams{
            .x = position.x + collision.hitbox_offset_x,
            .y = position.y + collision.hitbox_offset_y,
            .width = collision.hitbox_w,
            .height = collision.hitbox_h,
            .r = 120,
            .g = 155,
            .b = 120,
        }
    );
  }

  auto view_active = ctx.ecs.view<components::Position, components::CollisionActive>();
  for (auto [entity, position, collision] : view_active.each()) {
    renderer.draw_rect(
        framework::DrawRectParams{
            .x = position.x + collision.hitbox_offset_x,
            .y = position.y + collision.hitbox_offset_y,
            .width = collision.hitbox_w,
            .height = collision.hitbox_h,
            .r = 180,
            .g = 0,
            .b = 180,
        }
    );
  }
}

} // namespace systems