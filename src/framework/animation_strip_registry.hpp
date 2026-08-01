#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace framework {

struct AnimationFrame {
    int x;
    int y;
};

using AnimationStripID = uint8_t;
using AnimationStrip = std::vector<AnimationFrame>;

class AnimationStripRegistry {
  private:
    AnimationStripID cur_id = 0;
    std::unordered_map<AnimationStripID, AnimationStrip> animations;

  public:
    AnimationStripID create();
    void add_frame(AnimationStripID id, AnimationFrame frame);
    AnimationStrip get(AnimationStripID id) const;
    void clear();
};

} // namespace framework