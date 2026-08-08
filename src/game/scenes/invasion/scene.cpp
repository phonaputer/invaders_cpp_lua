#include "game/scenes/invasion/scene.hpp"
#include "framework/scene.hpp"
#include "game/scenes/invasion/components/game_over.hpp"
#include "game/scenes/invasion/components/hud.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include "game/scenes/invasion/script_api.hpp"
#include "game/scenes/invasion/systems/animation.hpp"
#include "game/scenes/invasion/systems/collision_callback.hpp"
#include "game/scenes/invasion/systems/collision_detection.hpp"
#include "game/scenes/invasion/systems/damage.hpp"
#include "game/scenes/invasion/systems/deletion.hpp"
#include "game/scenes/invasion/systems/hud_rendering.hpp"
#include "game/scenes/invasion/systems/player_attack.hpp"
#include "game/scenes/invasion/systems/player_movement.hpp"
#include "game/scenes/invasion/systems/position_following.hpp"
#include "game/scenes/invasion/systems/sprite_rendering.hpp"
#include "game/scenes/invasion/systems/velocity.hpp"
#include <memory>

namespace invasion {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_images_in_dir_png("invasion");

  ctx.systems.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.systems.add_update_system(std::make_unique<systems::CollisionCallback>(ctx.scripts, callback_registry));
  ctx.systems.add_update_system(std::make_unique<systems::Damage>());
  ctx.systems.add_update_system(std::make_unique<systems::Deletion>(ctx.scripts, callback_registry));
  ctx.systems.add_update_system(std::make_unique<systems::Velocity>());
  ctx.systems.add_update_system(std::make_unique<systems::PlayerMovement>());
  ctx.systems.add_update_system(std::make_unique<systems::PlayerAttack>(ctx.scripts, callback_registry));
  ctx.systems.add_update_system(std::make_unique<systems::PositionFollowing>());
  ctx.systems.add_update_system(std::make_unique<systems::Animation>(ctx.animation_strips));

  ctx.systems.add_draw_system(std::make_unique<systems::HUDRendering>(ctx.renderer));
  ctx.systems.add_draw_system(std::make_unique<systems::SpriteRendering>(ctx.renderer));

  ctx.ecs.ctx().emplace<components::HUD>();
  ctx.ecs.ctx().emplace<components::GameOver>();

  register_cpp_api_to_script_env(ctx);
  infra::add_callback_registry_to_script_env(ctx.scripts, callback_registry);

  ctx.scripts.call_function("invasion", "setScene");
}

} // namespace invasion