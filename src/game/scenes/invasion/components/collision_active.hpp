#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace components {

struct CollisionActive {
    float hitbox_offset_x = 0.0F;
    float hitbox_offset_y = 0.0F;
    float hitbox_w = 0.0F;
    float hitbox_h = 0.0F;
};

} // namespace components