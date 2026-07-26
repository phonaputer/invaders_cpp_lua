#pragma once

#include <string>

namespace framework {

class AudioPlayer {
  public:
    virtual ~AudioPlayer() = default;
    virtual void play_sound(std::string sound_id) = 0;
    virtual void stop_sound(std::string sound_id) = 0;
};

} // namespace framework