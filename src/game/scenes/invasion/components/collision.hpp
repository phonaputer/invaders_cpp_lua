#pragma once

namespace components {

struct Collision {
    float hitbox_offset_x = 0.0F;
    float hitbox_offset_y = 0.0F;
    float hitbox_w = 0.0F;
    float hitbox_h = 0.0F;

    // Optimization to avoid checking collision for entities that, for example, don't move.
    //
    // If two "passive" entities touch one another, a hit will not be registered.
    // But if either or both are "active" entities, a hit will be registered.
    bool is_passive = true;
};

} // namespace components