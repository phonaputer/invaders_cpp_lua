#pragma once

#include <cstdint>

namespace components {

struct TTL {
    uint16_t ticks_to_live = 0;
    uint16_t tick_counter = 0;
};

} // namespace components