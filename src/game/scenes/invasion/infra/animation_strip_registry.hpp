#pragma once

#include "framework/script_environment.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace infra {

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
};

void add_animation_strip_registry_to_script_env(
    framework::ScriptEnvironment &scripts, AnimationStripRegistry &animation_strips
);

} // namespace infra