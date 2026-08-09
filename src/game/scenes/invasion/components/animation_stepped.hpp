#pragma once

#include "framework/animation_strip_registry.hpp"
#include <cstdint>

namespace components {

struct AnimationStepped {
    uint8_t cur_frame = 0;
    framework::AnimationStripID strip_id = 0;

    bool operator==(const AnimationStepped &) const = default;
};

} // namespace components