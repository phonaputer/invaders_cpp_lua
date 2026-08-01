#pragma once

#include <string>

namespace framework {

class AudioPlayer {
  public:
    AudioPlayer() = default;
    virtual ~AudioPlayer() = default;
    AudioPlayer(const AudioPlayer &) = delete;
    AudioPlayer &operator=(const AudioPlayer &) = delete;
    AudioPlayer(AudioPlayer &&) = delete;
    AudioPlayer &operator=(AudioPlayer &&) = delete;

    virtual void play_sound(const std::string &sound_id) = 0;
    virtual void stop_sound(const std::string &sound_id) = 0;
};

} // namespace framework