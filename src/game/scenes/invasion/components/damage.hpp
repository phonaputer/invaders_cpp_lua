#pragma once

#include "game/scenes/invasion/components/damage_type_enum.hpp"
#include <cstdint>

namespace components {

struct Damage {
    DamageTypeSet type;
    uint8_t amount;
};

} // namespace components