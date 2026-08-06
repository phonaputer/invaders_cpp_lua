#pragma once

#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/util/text_renderer.hpp"

namespace systems {

class HUDRendering : public framework::System {
  private:
    framework::Renderer &renderer;
    util::TextRenderer text_renderer;

  public:
    explicit HUDRendering(framework::Renderer &renderer);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems