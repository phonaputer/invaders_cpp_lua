#pragma once

#include "framework/audio_player.hpp"
#include "framework/system.hpp"
#include <functional>
#include <set>
#include <string>

namespace systems {

class AudioPlaying : public framework::System {
  private:
    std::reference_wrapper<framework::AudioPlayer> audio_player;
    std::set<std::string> sounds_to_play;
    std::set<std::string> sounds_to_stop;

  public:
    explicit AudioPlaying(framework::AudioPlayer &audio_player);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems