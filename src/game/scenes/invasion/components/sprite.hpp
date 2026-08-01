#pragma once

#include <string>

namespace components {

struct Sprite {
    std::string src_id;
    float src_x = 0.0F;
    float src_y = 0.0F;
    float src_w = 0.0F;
    float src_h = 0.0F;
    float dst_w = 0.0F;
    float dst_h = 0.0F;
};

} // namespace components