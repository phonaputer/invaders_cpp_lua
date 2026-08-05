#pragma once

#include <cstdint>
#include <entt.hpp>

namespace components {

struct PositionFollowing {
    uint32_t leader = 0;
    float x_offset = 0;
    float y_offset = 0;
};

} // namespace components