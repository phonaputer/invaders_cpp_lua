#include "game/scenes/invasion/script_api.hpp"
#include "framework/scene.hpp"
#include "game/scenes/invasion/components/animation.hpp"
#include "game/scenes/invasion/components/collision_active.hpp"
#include "game/scenes/invasion/components/collision_callback.hpp"
#include "game/scenes/invasion/components/collision_passive.hpp"
#include "game/scenes/invasion/components/damage.hpp"
#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include "game/scenes/invasion/components/deletion_callback.hpp"
#include "game/scenes/invasion/components/hitpoints.hpp"
#include "game/scenes/invasion/components/hud.hpp"
#include "game/scenes/invasion/components/invader_animation.hpp"
#include "game/scenes/invasion/components/invader_orchestration_state.hpp"
#include "game/scenes/invasion/components/orchestrated_invader.hpp"
#include "game/scenes/invasion/components/player_attack.hpp"
#include "game/scenes/invasion/components/player_movement.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/position_following.hpp"
#include "game/scenes/invasion/components/sprite.hpp"
#include "game/scenes/invasion/components/to_be_deleted.hpp"
#include "game/scenes/invasion/components/ttl.hpp"
#include "game/scenes/invasion/components/velocity.hpp"
#include <algorithm>
#include <entt.hpp>

#include <LuaBridge/LuaBridge.h>

namespace invasion {

void register_all_components_to_script_env(framework::SceneInitializationContext &ctx) {
  // clang-format off
  ctx.scripts.register_component<components::Animation>(ctx.ecs, "Animation", [](auto &clazz) {
    clazz.addProperty("curFrame", &components::Animation::cur_frame, &components::Animation::cur_frame)
      .addProperty("tickCounter", &components::Animation::tick_counter, &components::Animation::tick_counter)
      .addProperty("ticksPerFrame", &components::Animation::ticks_per_frame, &components::Animation::ticks_per_frame)
      .addProperty("stripID", &components::Animation::strip_id, &components::Animation::strip_id)
      .addProperty("playing", &components::Animation::playing, &components::Animation::playing)
      .addProperty("playReversed", &components::Animation::play_reversed, &components::Animation::play_reversed);
  });
  ctx.scripts.register_component<components::CollisionActive>(ctx.ecs, "CollisionActive", [](auto &clazz) {
    clazz.addProperty("hitboxOffsetX", &components::CollisionActive::hitbox_offset_x, &components::CollisionActive::hitbox_offset_x)
      .addProperty("hitboxOffsetY", &components::CollisionActive::hitbox_offset_y, &components::CollisionActive::hitbox_offset_y)
      .addProperty("hitboxW", &components::CollisionActive::hitbox_w, &components::CollisionActive::hitbox_w)
      .addProperty("hitboxH", &components::CollisionActive::hitbox_h, &components::CollisionActive::hitbox_h);
  });
  ctx.scripts.register_component<components::CollisionPassive>(ctx.ecs, "CollisionPassive", [](auto &clazz) {
    clazz.addProperty("hitboxOffsetX", &components::CollisionPassive::hitbox_offset_x, &components::CollisionPassive::hitbox_offset_x)
      .addProperty("hitboxOffsetY", &components::CollisionPassive::hitbox_offset_y, &components::CollisionPassive::hitbox_offset_y)
      .addProperty("hitboxW", &components::CollisionPassive::hitbox_w, &components::CollisionPassive::hitbox_w)
      .addProperty("hitboxH", &components::CollisionPassive::hitbox_h, &components::CollisionPassive::hitbox_h);
  });
  ctx.scripts.register_component<components::CollisionCallback>(ctx.ecs, "CollisionCallback", [](auto &clazz) {
    clazz.addProperty("callback", &components::CollisionCallback::callback, &components::CollisionCallback::callback);
  });
  ctx.scripts.register_component<components::Damage>(ctx.ecs, "Damage", [](auto &clazz) {
    clazz.addProperty("type", &components::Damage::type, &components::Damage::type)
      .addProperty("amount", &components::Damage::amount, &components::Damage::amount);
  });
  ctx.scripts.register_component<components::DeletionCallback>(ctx.ecs, "DeletionCallback", [](auto &clazz) {
    clazz.addProperty("callback", &components::DeletionCallback::callback, &components::DeletionCallback::callback);
  });
  ctx.scripts.register_component<components::Hitpoints>(ctx.ecs, "Hitpoints", [](auto &clazz) {
    clazz.addProperty("susceptibleTo", &components::Hitpoints::susceptible_to, &components::Hitpoints::susceptible_to)
      .addProperty("curHitpoints", &components::Hitpoints::cur_hitpoints, &components::Hitpoints::cur_hitpoints);
  });
  ctx.scripts.register_component<components::InvaderAnimation>(ctx.ecs, "InvaderAnimation", [](auto &clazz) {
    clazz.addProperty("curFrame", &components::InvaderAnimation::cur_frame, &components::InvaderAnimation::cur_frame)
      .addProperty("stripID", &components::InvaderAnimation::strip_id, &components::InvaderAnimation::strip_id);
  });
  ctx.scripts.register_singleton_component<components::InvaderOrchestrationState>(ctx.ecs, "InvaderOrchestrationState", [](auto &clazz) {
    clazz.addProperty("noInvadersCallback", &components::InvaderOrchestrationState::no_invaders_callback, &components::InvaderOrchestrationState::no_invaders_callback)
      .addProperty("shootCallback", &components::InvaderOrchestrationState::shoot_callback, &components::InvaderOrchestrationState::shoot_callback)
      .addProperty("baseTicksPerMove", &components::InvaderOrchestrationState::base_ticks_per_move, &components::InvaderOrchestrationState::base_ticks_per_move)
      .addProperty("tickCounter", &components::InvaderOrchestrationState::tick_counter, &components::InvaderOrchestrationState::tick_counter)
      .addProperty("xSpeed", &components::InvaderOrchestrationState::x_speed, &components::InvaderOrchestrationState::x_speed)
      .addProperty("ySpeed", &components::InvaderOrchestrationState::y_speed, &components::InvaderOrchestrationState::y_speed)
      .addProperty("lastInvaderXSpeed", &components::InvaderOrchestrationState::last_invader_x_speed, &components::InvaderOrchestrationState::last_invader_x_speed)
      .addProperty("lastInvaderYSpeed", &components::InvaderOrchestrationState::last_invader_y_speed, &components::InvaderOrchestrationState::last_invader_y_speed)
      .addProperty("ticksPerShot", &components::InvaderOrchestrationState::ticks_per_shot, &components::InvaderOrchestrationState::ticks_per_shot)
      .addProperty("shootTickCounter", &components::InvaderOrchestrationState::shoot_tick_counter, &components::InvaderOrchestrationState::shoot_tick_counter)
      .addProperty("movingLeft", &components::InvaderOrchestrationState::moving_left, &components::InvaderOrchestrationState::moving_left);
  });
  ctx.scripts.register_component<components::OrchestratedInvader>(ctx.ecs, "OrchestratedInvader", []([[maybe_unused]] auto &clazz) {
  });
  ctx.scripts.register_component<components::PlayerAttack>(ctx.ecs, "PlayerAttack", [](auto &clazz) {
    clazz.addProperty("ticksPerAttack", &components::PlayerAttack::ticks_per_attack, &components::PlayerAttack::ticks_per_attack)
      .addProperty("tickCounter", &components::PlayerAttack::tick_counter, &components::PlayerAttack::tick_counter)
      .addProperty("callback", &components::PlayerAttack::callback, &components::PlayerAttack::callback);
  });
  ctx.scripts.register_component<components::PlayerMovement>(ctx.ecs, "PlayerMovement", [](auto &clazz) {
    clazz.addProperty("xSpeed", &components::PlayerMovement::x_speed, &components::PlayerMovement::x_speed);
  });
  ctx.scripts.register_component<components::Position>(ctx.ecs, "Position", [](auto &clazz) {
    clazz.addProperty("x", &components::Position::x, &components::Position::x)
      .addProperty("y", &components::Position::y, &components::Position::y)
      .addProperty("w", &components::Position::y, &components::Position::w)
      .addProperty("h", &components::Position::y, &components::Position::h)
      .addProperty("z", &components::Position::z, &components::Position::z);
  });
  ctx.scripts.register_component<components::PositionFollowing>(ctx.ecs, "PositionFollowing", [](auto &clazz) {
    clazz.addProperty("leader", &components::PositionFollowing::leader, &components::PositionFollowing::leader)
      .addProperty("xOffset", &components::PositionFollowing::x_offset, &components::PositionFollowing::x_offset)
      .addProperty("yOffset", &components::PositionFollowing::y_offset, &components::PositionFollowing::y_offset);
  });
  ctx.scripts.register_component<components::Sprite>(ctx.ecs, "Sprite", [](auto &clazz) {
    clazz.addProperty("srcID", &components::Sprite::src_id, &components::Sprite::src_id)
      .addProperty("srcX", &components::Sprite::src_x, &components::Sprite::src_x)
      .addProperty("srcY", &components::Sprite::src_y, &components::Sprite::src_y)
      .addProperty("srcW", &components::Sprite::src_w, &components::Sprite::src_w)
      .addProperty("srcH", &components::Sprite::src_h, &components::Sprite::src_h)
      .addProperty("dstW", &components::Sprite::dst_w, &components::Sprite::dst_w)
      .addProperty("dstH", &components::Sprite::dst_h, &components::Sprite::dst_h);
  });
  ctx.scripts.register_component<components::ToBeDeleted>(ctx.ecs, "ToBeDeleted", []([[maybe_unused]] auto &clazz) {
  });
  ctx.scripts.register_component<components::TTL>(ctx.ecs, "TTL", [](auto &clazz) {
    clazz.addProperty("ticksToLive", &components::TTL::ticks_to_live, &components::TTL::ticks_to_live)
      .addProperty("tickCounter", &components::TTL::tick_counter, &components::TTL::tick_counter);
  });
  ctx.scripts.register_component<components::Velocity>(ctx.ecs, "Velocity", [](auto &clazz) {
    clazz.addProperty("x", &components::Velocity::x, &components::Velocity::x)
      .addProperty("y", &components::Velocity::y, &components::Velocity::y);
  });
  // clang-format on
}

void increment_score(entt::registry &ecs, int amount) {
  auto &hud = ecs.ctx().get<components::HUD>();
  hud.score += amount;
  hud.high_score = std::max(hud.high_score, hud.score);
}

void register_cpp_api_to_script_env(framework::SceneInitializationContext &ctx) {
  register_all_components_to_script_env(ctx);
  components::register_damage_type_enum_to_script_env(ctx.scripts);

  ctx.scripts.register_function("HUD", "incrementScore", [&ecs = ctx.ecs](double amount) {
    increment_score(ecs, static_cast<int>(amount));
  });
}

} // namespace invasion