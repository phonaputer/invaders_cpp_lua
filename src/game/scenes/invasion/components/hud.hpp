#pragma once

#include "game/scenes/invasion/constants.hpp"
#include <cstdint>

namespace components {

struct HUD {
    uint32_t score = 0;
    uint32_t high_score = 0;
    uint8_t remaining_lives = invasion::STARTING_LIVES;
};

} // namespace components