#pragma once

#include "framework/animation_strip_registry.hpp"
#include <cstdint>

namespace components {

struct AnimationUnpausable {
    uint16_t tick_counter = 0;
    uint16_t ticks_per_frame = 0;
    uint8_t cur_frame = 0;
    framework::AnimationStripID strip_id = 0;

    bool operator==(const AnimationUnpausable &) const = default;
};

} // namespace components