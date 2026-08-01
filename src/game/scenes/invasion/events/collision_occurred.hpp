#pragma once

#include <entt.hpp>

namespace events {

struct CollisionOccurred {
    entt::entity who_am_i;
    entt::entity who_i_hit;

    bool operator==(const CollisionOccurred &) const = default;
};

} // namespace events