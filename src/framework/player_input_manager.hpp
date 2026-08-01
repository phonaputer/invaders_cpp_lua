#pragma once

#include <cstdint>
#include <set>

namespace framework {

enum class PlayerInput : uint8_t { LEFT, RIGHT, FIRE, UP, DOWN, PAUSE };

class PlayerInputManager {
  private:
    std::set<PlayerInput> current_inputs;
    std::set<PlayerInput> current_initiations;
    std::set<PlayerInput> previous_tick_inputs;

  public:
    [[nodiscard]] bool is_engaged(PlayerInput input) const;
    [[nodiscard]] bool is_initiated(PlayerInput input) const;
    void engage(PlayerInput input);
    void disengage(PlayerInput input);
    void update();
};

} // namespace framework