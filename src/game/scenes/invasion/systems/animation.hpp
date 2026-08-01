#pragma once

#include "framework/animation_strip_registry.hpp"
#include "framework/system.hpp"
#include <functional>

namespace systems {

class Animation : public framework::System {
  private:
    std::reference_wrapper<const framework::AnimationStripRegistry> animation_strips;

  public:
    explicit Animation(const framework::AnimationStripRegistry &animation_strips);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems
