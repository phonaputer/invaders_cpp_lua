#pragma once

#include "framework/event_broker.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/script_environment.hpp"
#include <entt.hpp>

namespace framework {

// Intentionally using references here to simplify the API.
// And anyways this is just a parameter object to clean up the execute function signature.
//
// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct ExecuteCtx {
    entt::registry &ecs;
    framework::EventBroker &events;
    const PlayerInputManager &player_input;
    framework::ScriptEnvironment &scripts;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

class System {
  public:
    System() = default;
    virtual ~System() = default;
    System(const System &) = delete;
    System &operator=(const System &) = delete;
    System(System &&) = delete;
    System &operator=(System &&) = delete;

    virtual void execute(ExecuteCtx &ctx) = 0;
};

} // namespace framework