#pragma once

#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include <cstdint>

namespace components {

struct Hitpoints {
    DamageTypeSet susceptible_to;
    uint8_t cur_hitpoints;
};

} // namespace components