#pragma once

#include "framework/animation_strip_registry.hpp"

namespace components {

struct Animation {
    unsigned int cur_frame = 0;
    unsigned int tick_counter = 0;
    unsigned int ticks_per_frame = 0;
    framework::AnimationStripID strip_id = 0;
    bool playing = true;
    bool play_reversed = false;

    bool operator==(const Animation &) const = default;
};

} // namespace components