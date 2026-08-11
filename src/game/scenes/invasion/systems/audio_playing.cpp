#include "game/scenes/invasion/systems/audio_playing.hpp"
#include "framework/audio_player.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/events/play_sound.hpp"
#include "game/scenes/invasion/events/stop_sound.hpp"

namespace systems {

AudioPlaying::AudioPlaying(framework::AudioPlayer &audio_player)
    : audio_player{audio_player} {
}

void AudioPlaying::execute(framework::ExecuteCtx &ctx) {
  sounds_to_play.clear();
  for (const auto &event : ctx.events.get_all_draw<events::PlaySound>()) {
    sounds_to_play.insert(event.audio_src);
  }

  sounds_to_stop.clear();
  for (const auto &event : ctx.events.get_all_draw<events::StopSound>()) {
    sounds_to_stop.insert(event.audio_src);
    sounds_to_play.erase(event.audio_src);
  }

  for (const auto &sound : sounds_to_play) {
    audio_player.get().play_sound(sound);
  }

  for (const auto &sound : sounds_to_stop) {
    audio_player.get().stop_sound(sound);
  }
}

} // namespace systems