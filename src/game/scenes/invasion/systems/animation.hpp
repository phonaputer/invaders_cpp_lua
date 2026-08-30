#pragma once

#include "framework/system.hpp"
#include "game/scenes/invasion/infra/animation_strip_registry.hpp"
#include <functional>

namespace systems {

class Animation : public framework::System {
  private:
    std::reference_wrapper<const infra::AnimationStripRegistry> animation_strips;

    void execute_unpausable_animations(framework::ExecuteCtx &ctx);
    void execute_regular_animations(framework::ExecuteCtx &ctx);

  public:
    explicit Animation(const infra::AnimationStripRegistry &animation_strips);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems
