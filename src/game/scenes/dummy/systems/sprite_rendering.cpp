#include "game/scenes/dummy/systems/sprite_rendering.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/dummy/components/position.hpp"
#include "game/scenes/dummy/components/sprite.hpp"

namespace systems {

SpriteRendering::SpriteRendering(framework::Renderer &renderer)
    : renderer{renderer} {
}

void SpriteRendering::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Position, components::Sprite>();
  view.use<components::Position>();

  for (auto [entity, position, sprite] : view.each()) {
    renderer.draw_image(
        framework::DrawImageParams{
            .src_id = sprite.src_id,
            .src_x = sprite.src_x,
            .src_y = sprite.src_y,
            .src_width = sprite.src_w,
            .src_height = sprite.src_h,
            .dst_x = position.x,
            .dst_y = position.y,
            .dst_width = sprite.dst_w,
            .dst_height = sprite.dst_h,
        }
    );
  }
}

} // namespace systems