#include "game/util/text_renderer.hpp"
#include "framework/constants.hpp"
#include "framework/renderer.hpp"
#include "text_renderer.hpp"
#include <cctype>
#include <string>
#include <unordered_map>

namespace util {

constexpr float CHARACTER_SRC_WIDTH = 8;
constexpr float CHARACTER_SRC_HEIGHT = 8;

constexpr float CHARACTER_DRAW_WIDTH = 8;
constexpr float CHARACTER_DRAW_HEIGHT = 8;

std::string image_src() {
  const std::string image_src = "space_invaders.png";
  return image_src;
}

struct Frame {
    float x;
    float y;
};

std::unordered_map<char, Frame> chars_to_frames() {
  // NOLINTBEGIN(modernize-use-designated-initializers)
  const std::unordered_map<char, Frame> chars_to_frames({
      {'1', {0, 10}},  {'2', {1, 10}},  {'3', {2, 10}},  {'4', {3, 10}},  {'5', {4, 10}},  {'6', {5, 10}},
      {'7', {6, 10}},  {'8', {7, 10}},  {'9', {8, 10}},  {'0', {9, 10}},  {'a', {0, 11}},  {'b', {1, 11}},
      {'c', {2, 11}},  {'d', {3, 11}},  {'e', {4, 11}},  {'f', {5, 11}},  {'g', {6, 11}},  {'h', {7, 11}},
      {'i', {8, 11}},  {'j', {9, 11}},  {'k', {10, 11}}, {'l', {11, 11}}, {'m', {0, 12}},  {'n', {1, 12}},
      {'o', {2, 12}},  {'p', {3, 12}},  {'q', {4, 12}},  {'r', {5, 12}},  {'s', {6, 12}},  {'t', {7, 12}},
      {'u', {8, 12}},  {'v', {9, 12}},  {'w', {10, 12}}, {'x', {11, 12}}, {'y', {12, 12}}, {'z', {13, 12}},
      {'-', {14, 12}}, {':', {15, 12}}, {'<', {15, 10}}, {'>', {15, 11}}, {'?', {15, 9}},
  });
  // NOLINTEND(modernize-use-designated-initializers)

  return chars_to_frames;
}

TextRenderer::TextRenderer(framework::Renderer &renderer)
    : renderer(renderer) {
}

void TextRenderer::render_text(float x, float y, const std::string &text) const {
  float cur_x = x;

  for (const char c : text) {
    const char c_normalized = static_cast<char>(std::tolower(c));

    const auto c2f = chars_to_frames();

    if (c2f.contains(c_normalized)) {
      auto frame = c2f.at(c_normalized);

      renderer.get().draw_image(
          framework::DrawImageParams{
              .src_id = image_src(),
              .src_x = CHARACTER_SRC_WIDTH * frame.x,
              .src_y = CHARACTER_SRC_WIDTH * frame.y,
              .src_width = CHARACTER_SRC_WIDTH,
              .src_height = CHARACTER_SRC_HEIGHT,
              .dst_x = cur_x,
              .dst_y = y,
              .dst_width = CHARACTER_DRAW_WIDTH,
              .dst_height = CHARACTER_DRAW_HEIGHT,
          }
      );
    }

    cur_x += CHARACTER_X_SPACING;
  }
}

void TextRenderer::render_text_centered(float y, const std::string &text) const {
  const float text_width_px = static_cast<float>(text.length()) * TextRenderer::CHARACTER_X_SPACING;

  const float start_x = (framework::WINDOW_WIDTH * 0.5F) - (text_width_px * 0.5F);

  render_text(start_x, y, text);
}

} // namespace util