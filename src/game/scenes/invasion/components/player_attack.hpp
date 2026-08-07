#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace components {

struct PlayerAttack {
    int ticks_per_attack = 0;
    int tick_counter = 0;
    infra::CallbackID callback = 0;

    bool operator==(const PlayerAttack &) const = default;
};

} 