#include "framework/animation_strip_registry.hpp"
#include <cassert>

namespace framework {

AnimationStripID AnimationStripRegistry::create() {
  auto result = ++cur_id;

  animations.try_emplace(result);

  return result;
}

void AnimationStripRegistry::add_frame(AnimationStripID id, AnimationFrame frame) {
  assert(animations.contains(id) && "AnimationStrip to add frame to does not exist");
  if (animations.contains(id)) {
    animations.at(id).push_back(frame);
  }
}

AnimationStrip AnimationStripRegistry::get(AnimationStripID id) const {
  assert(animations.contains(id) && "Requested AnimationStrip does not exist");
  if (animations.contains(id)) {
    return animations.at(id);
  }

  return {};
}

void AnimationStripRegistry::clear() {
  animations.clear();
}

} // namespace framework