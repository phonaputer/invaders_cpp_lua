#include "game/scenes/invasion/systems/collision_detection.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/events/collision_occurred.hpp"
#include <cmath>
#include <cstddef>
#include <set>
#include <vector>

namespace systems {

void CollisionDetection::execute(framework::ExecuteCtx &ctx) {
  fill_buckets(ctx.ecs);

  for (const auto &key : buckets_to_check) {
    check_collisions(ctx, hitbox_buckets.at(key.x).at(key.y));
  }

  clear_buckets();
}

BucketKey CollisionDetection::to_bucket_key(float x, float y) {
  return {
      .x = static_cast<int>(x) / BUCKET_WIDTH,
      .y = static_cast<int>(y) / BUCKET_HEIGHT,
  };
}

void CollisionDetection::fill_buckets(entt::registry &ecs) {
  auto view = ecs.view<const components::Collision, const components::Position>();

  for (auto [entity, collision, position] : view.each()) {
    auto hitbox = Hitbox{
        .entity = entity,
        .x = collision.hitbox_offset_x + position.x,
        .y = collision.hitbox_offset_y + position.y,
        .w = collision.hitbox_w,
        .h = collision.hitbox_h,
        .is_passive = collision.is_passive,
    };

    const auto min_bucket = to_bucket_key(hitbox.x, hitbox.y);
    const auto max_bucket = to_bucket_key(hitbox.x + hitbox.w, hitbox.y + hitbox.h);

    for (auto bucket_x = min_bucket.x; bucket_x <= max_bucket.x; bucket_x++) {
      for (auto bucket_y = min_bucket.y; bucket_y <= max_bucket.y; bucket_y++) {
        add_to_bucket(hitbox, bucket_x, bucket_y);
        if (!collision.is_passive) {
          buckets_to_check.insert(BucketKey{.x = bucket_x, .y = bucket_y});
        }
      }
    }
  }
}

void CollisionDetection::add_to_bucket(Hitbox hitbox, int bucket_x, int bucket_y) {
  auto &y_bucket = hitbox_buckets[bucket_x];
  auto &cell_bucket = y_bucket[bucket_y];

  cell_bucket.push_back(hitbox);
}

// It's ok to clear these without deleting them from the map since the size of
// the canvas never changes meaning the list of buckets is fixed.
void CollisionDetection::clear_buckets() {
  for (auto &[_, y_bucket] : hitbox_buckets) {
    for (auto &[_, cell_bucket] : y_bucket) {
      cell_bucket.clear();
    }
  }

  buckets_to_check.clear();
}

void CollisionDetection::check_collisions(framework::ExecuteCtx &ctx, const std::vector<Hitbox> &hitboxes) {
  for (size_t l = 0; l < hitboxes.size(); l++) {
    auto left = hitboxes.at(l);

    for (size_t r = l + 1; r < hitboxes.size(); r++) {
      auto right = hitboxes.at(r);

      if (are_touching(left, right)) {
        emit_collided_if_didnt_already(ctx, left.entity, right.entity);
      }
    }
  }
}

void CollisionDetection::emit_collided_if_didnt_already(
    framework::ExecuteCtx &ctx, entt::entity right, entt::entity left
) {
  for (const auto &event : ctx.events.get_all<events::CollisionOccurred>()) {
    if (event.who_am_i == right && event.who_i_hit == left) {
      return;
    }
    if (event.who_am_i == left && event.who_i_hit == right) {
      return;
    }
  }

  ctx.events.push_back(events::CollisionOccurred{.who_am_i = left, .who_i_hit = right});
  ctx.events.push_back(events::CollisionOccurred{.who_am_i = right, .who_i_hit = left});
}

bool CollisionDetection::are_touching(Hitbox left, Hitbox right) {
  if (right.is_passive && left.is_passive) {
    return false;
  }

  // "left" above "right"
  if (left.y + left.h < right.y) {
    return false;
  }

  // "left" below "right"
  if (left.y > right.y + right.h) {
    return false;
  }

  // "left" left of "right"
  if (left.x + left.w < right.x) {
    return false;
  }

  // "left" right of "right"
  if (left.x > right.x + right.w) {
    return false;
  }

  return true;
}

} // namespace systems