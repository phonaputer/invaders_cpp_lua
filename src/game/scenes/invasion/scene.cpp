#include "game/scenes/invasion/scene.hpp"
#include "framework/scene.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/systems/collision_detection.hpp"
#include "game/scenes/invasion/systems/sprite_rendering.hpp"
#include "game/scenes/invasion/systems/velocity.hpp"
#include <memory>

namespace invasion {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_images_in_dir_png("invasion");

  ctx.systems.add_update_system(std::make_unique<systems::Velocity>());
  ctx.systems.add_update_system(std::make_unique<systems::CollisionDetection>());

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
          .src_id = "space_invaders.png",
          .src_x = 16,
          .src_y = 0,
          .src_w = 16,
          .src_h = 16,
          .dst_w = 16,
          .dst_h = 16,
      }
  );
}

} // namespace invasion