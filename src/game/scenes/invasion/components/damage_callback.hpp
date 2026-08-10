#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace components {

struct DamageCallback {
    infra::CallbackID callback = 0;
};

} // namespace components