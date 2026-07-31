#pragma once

#include <string>

namespace components {

struct Sprite {
    std::string src_id;
    float src_x;
    float src_y;
    float src_w;
    float src_h;
    float dst_w;
    float dst_h;
};

} // namespace components