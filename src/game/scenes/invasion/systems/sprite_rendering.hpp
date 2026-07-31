#pragma once

#include "framework/renderer.hpp"
#include "framework/system.hpp"

namespace systems {

class SpriteRendering : public framework::System {
  private:
    framework::Renderer &renderer;

  public:
    explicit SpriteRendering(framework::Renderer &renderer);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems