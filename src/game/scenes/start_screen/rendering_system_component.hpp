#pragma once

namespace start_screen {

struct RenderingSystemComponent {
    int game_start_tick_counter = 0;
    bool game_start_engaged = false;
    bool game_start_blink = true;
};

} // namespace start_screen