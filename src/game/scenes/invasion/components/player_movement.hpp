#pragma once

#include "framework/animation_strip_registry.hpp"
#include <cstdint>

namespace components {

struct PlayerMovement {
    uint8_t x_speed = 0;

    bool operator==(const PlayerMovement &) const = default;
};

} // namespace components