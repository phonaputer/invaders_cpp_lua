#pragma once

#include <string>

namespace framework {

struct DrawImageParams {
    std::string src_id;
    float src_x;
    float src_y;
    float src_width;
    float src_height;
    float dst_x;
    float dst_y;
    float dst_width;
    float dst_height;

    auto operator<=>(const DrawImageParams &) const = default;
};

struct DrawRectParams {
    float x;
    float y;
    float width;
    float height;
    unsigned int r;
    unsigned int g;
    unsigned int b;
};

struct DrawLineParams {
    float start_x;
    float start_y;
    float end_x;
    float end_y;
    unsigned int r;
    unsigned int g;
    unsigned int b;
};

class Renderer {
  public:
    virtual ~Renderer() = default;
    virtual void draw_image(const DrawImageParams &params) = 0;
    virtual void draw_rect(const DrawRectParams &params) = 0;
    virtual void draw_line(const DrawLineParams &params) = 0;
};

} // namespace framework