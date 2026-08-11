#pragma once

#include "framework/animation_strip_registry.hpp"
#include "framework/script_environment.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/invader_orchestration_state.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>
#include <functional>

namespace systems {

class InvaderOrchestration : public framework::System {
  private:
    std::reference_wrapper<framework::ScriptEnvironment> scripts;
    std::reference_wrapper<infra::CallbackGetter> callbacks;
    std::reference_wrapper<framework::AnimationStripRegistry> animation_strips;

    static size_t count_invaders(framework::ExecuteCtx &ctx);
    void invoke_no_invaders_callback(components::InvaderOrchestrationState &state);
    static bool should_move_this_tick(
        components::InvaderOrchestrationState &state, const size_t invader_count
    );
    void move_invaders(
        framework::ExecuteCtx &ctx, components::InvaderOrchestrationState &state, const size_t invader_count
    );
    bool handle_wall_hit_if_any(
        framework::ExecuteCtx &ctx, components::InvaderOrchestrationState &state, const size_t invader_count
    );
    void invoke_touchdown_callback(components::InvaderOrchestrationState &state);
    void animate_invaders(framework::ExecuteCtx &ctx);
    void shoot(components::InvaderOrchestrationState &state);

  public:
    InvaderOrchestration(
        framework::ScriptEnvironment &scripts,
        infra::CallbackGetter &callbacks,
        framework::AnimationStripRegistry &animation_strips
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems