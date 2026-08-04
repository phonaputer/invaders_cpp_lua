#pragma once

#include <string>

namespace components {

struct PlayerAttack {
    int ticks_per_attack = 0;
    int tick_counter = 0;
    std::string callback;
};

} 