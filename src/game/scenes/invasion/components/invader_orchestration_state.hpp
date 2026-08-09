#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>

namespace components {

struct InvaderOrchestrationState {
    infra::CallbackID no_invaders_callback = 0;
    uint8_t base_ticks_per_move = 0;
    uint8_t tick_counter = 0;
    uint8_t x_speed = 0;
    uint8_t y_speed = 0;
    uint8_t last_invader_x_speed = 0;
    uint8_t last_invader_y_speed = 0;
    bool moving_left = false;
};

} // namespace components