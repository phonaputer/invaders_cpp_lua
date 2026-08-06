#pragma once

#include "framework/renderer.hpp"
#include <functional>
#include <string>

namespace util {

class TextRenderer {
  private:
    std::reference_wrapper<framework::Renderer> renderer;

  public:
    static constexpr float CHARACTER_X_SPACING = 6;

    explicit TextRenderer(framework::Renderer &renderer);
    void render_text(float x, float y, const std::string &text) const;
    void render_text_centered(float y, const std::string &text) const;
};

} // namespace util