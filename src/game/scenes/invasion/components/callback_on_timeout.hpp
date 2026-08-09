#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>

namespace components {

struct CallbackOnTimeout {
    infra::CallbackID callback = 0;
    uint16_t timeout_ticks = 0;
    uint16_t tick_counter = 0;

    bool operator==(const CallbackOnTimeout &) const = default;
};

} // namespace components