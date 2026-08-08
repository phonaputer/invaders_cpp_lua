#pragma once

#include "game/scenes/invasion/infra/callback_registry.hpp"

namespace components {

struct CollisionCallback {
    infra::CallbackID callback = 0;
};

} // namespace components