#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>

namespace components {

struct CallbackOnTimeoutUnpausable {
    infra::CallbackID callback = 0;
    uint16_t timeout_ticks = 0;
    uint16_t tick_counter = 0;

    bool operator==(const CallbackOnTimeoutUnpausable &) const = default;
};

} // namespace components