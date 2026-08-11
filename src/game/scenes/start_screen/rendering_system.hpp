#pragma once

#include "framework/audio_player.hpp"
#include "framework/renderer.hpp"
#include "framework/scene_setter.hpp"
#include "framework/system.hpp"
#include "game/util/text_renderer.hpp"

namespace start_screen {

class RenderingSystem : public framework::System {
  private:
    static constexpr int GAME_START_TOTAL_TICKS = 30;
    static constexpr int GAME_START_BLINK_TICKS = 3;

    framework::SceneSetter &scene_setter;

    framework::AudioPlayer &audio_player;

    framework::Renderer &renderer;
    util::TextRenderer text_renderer;

  public:
    RenderingSystem(
        framework::SceneSetter &scene_setter,
        framework::AudioPlayer &audio_player,
        framework::Renderer &renderer
    );
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace start_screen