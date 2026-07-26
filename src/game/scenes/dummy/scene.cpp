#include "game/scenes/dummy/scene.hpp"
#include "game/scenes/dummy/components/position.hpp"
#include "game/scenes/dummy/components/sprite.hpp"
#include "game/scenes/dummy/systems/sprite_rendering.hpp"
#include <memory>

namespace dummy {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("spritesheet", "./assets/space_invaders.png");

  ctx.systems.add_draw_system(std::make_unique<systems::SpriteRendering>(ctx.renderer));

  auto entity = ctx.ecs.create();
  ctx.ecs.emplace<components::Position>(
      entity,
      components::Position{
          .x = 25,
          .y = 25,
          .z = 1,
      }
  );
  ctx.ecs.emplace<components::Sprite>(
      entity,
      components::Sprite{
          .src_id = "spritesheet",
          .src_x = 16,
          .src_y = 0,
          .src_w = 16,
          .src_h = 16,
          .dst_w = 16,
          .dst_h = 16,
      }
  );
}

} // namespace dummy