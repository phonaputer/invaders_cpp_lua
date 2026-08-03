#include "game/scenes/invasion/scene.hpp"
#include "framework/scene.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/velocity.hpp"
#include "game/scenes/invasion/systems/animation.hpp"
#include "game/scenes/invasion/systems/collision_detection.hpp"
#include "game/scenes/invasion/systems/deletion.hpp"
#include "game/scenes/invasion/systems/player_movement.hpp"
#include "game/scenes/invasion/systems/sprite_rendering.hpp"
#include "game/scenes/invasion/systems/velocity.hpp"
#include <memory>

namespace invasion {

void register_all_components_to_script_env(framework::SceneInitializationContext ctx) {
  // clang-format off
  ctx.scripts.register_component<components::Animation>("Animation", [](auto &clazz) {
    clazz.addProperty("curFrame", &components::Animation::cur_frame, &components::Animation::cur_frame)
        .addProperty("tickCounter", &components::Animation::tick_counter, &components::Animation::tick_counter)
        .addProperty("ticksPerFrame", &components::Animation::ticks_per_frame, &components::Animation::ticks_per_frame)
        .addProperty("stripID", &components::Animation::strip_id, &components::Animation::strip_id)
        .addProperty("playing", &components::Animation::playing, &components::Animation::playing)
        .addProperty("playReversed", &components::Animation::play_reversed, &components::Animation::play_reversed);
  });
  ctx.scripts.register_component<components::Collision>("Collision", [](auto &clazz) {
    clazz.addProperty("hitboxOffsetX", &components::Collision::hitbox_offset_x, &components::Collision::hitbox_offset_x)
        .addProperty("hitboxOffsetY", &components::Collision::hitbox_offset_y, &components::Collision::hitbox_offset_y)
        .addProperty("hitboxW", &components::Collision::hitbox_w, &components::Collision::hitbox_w)
        .addProperty("hitboxH", &components::Collision::hitbox_h, &components::Collision::hitbox_h)
        .addProperty("isPassive", &components::Collision::is_passive, &components::Collision::is_passive);
  });
  ctx.scripts.register_component<components::PlayerMovement>("PlayerMovement", [](auto &clazz) {
    clazz.addProperty("xSpeed", &components::PlayerMovement::x_speed, &components::PlayerMovement::x_speed);
  });
  ctx.scripts.register_component<components::Position>("Position", [](auto &clazz) {
    clazz.addProperty("x", &components::Position::x, &components::Position::x)
        .addProperty("y", &components::Position::y, &components::Position::y)
        .addProperty("w", &components::Position::y, &components::Position::w)
        .addProperty("h", &components::Position::y, &components::Position::h)
        .addProperty("z", &components::Position::z, &components::Position::z);
  });
  ctx.scripts.register_component<components::Sprite>("Sprite", [](auto &clazz) {
    clazz.addProperty("srcID", &components::Sprite::src_id, &components::Sprite::src_id)
        .addProperty("srcX", &components::Sprite::src_x, &components::Sprite::src_x)
        .addProperty("srcY", &components::Sprite::src_y, &components::Sprite::src_y)
        .addProperty("srcW", &components::Sprite::src_w, &components::Sprite::src_w)
        .addProperty("srcH", &components::Sprite::src_h, &components::Sprite::src_h)
        .addProperty("dstW", &components::Sprite::dst_w, &components::Sprite::dst_w)
        .addProperty("dstH", &components::Sprite::dst_h, &components::Sprite::dst_h);
  });
  ctx.scripts.register_component<components::ToBeDeleted>("ToBeDeleted", []([[maybe_unused]] auto &clazz) {
  });
  ctx.scripts.register_component<components::Velocity>("Velocity", [](auto &clazz) {
    clazz.addProperty("x", &components::Velocity::x, &components::Velocity::x)
        .addProperty("y", &components::Velocity::y, &components::Velocity::y);
  });
  // clang-format on
}

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_images_in_dir_png("invasion");

  ctx.systems.add_update_system(std::make_unique<systems::Velocity>());
  ctx.systems.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.systems.add_update_system(std::make_unique<systems::Deletion>());
  ctx.systems.add_update_system(std::make_unique<systems::Animation>(ctx.animation_strips));
  ctx.systems.add_update_system(std::make_unique<systems::PlayerMovement>());

  ctx.systems.add_draw_system(std::make_unique<systems::SpriteRendering>(ctx.renderer));

  register_all_components_to_script_env(ctx);
  ctx.scripts.exec_package("invasion");
  ctx.scripts.call_global_function("setScene");
}

} // namespace invasion