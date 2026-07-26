#pragma once

#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include <entt.hpp>

namespace framework {

struct ExecuteCtx {
    entt::registry &ecs;
    framework::EventBroker &events;
    const PlayerInputManager &player_input;
};

class System {
  public:
    virtual ~System() = default;
    virtual void execute(ExecuteCtx &ctx) = 0;
};

} // namespace framework