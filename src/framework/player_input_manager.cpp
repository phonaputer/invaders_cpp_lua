#include "framework/player_input_manager.hpp"

namespace framework {

bool PlayerInputManager::is_engaged(PlayerInput input) const {
  return current_inputs.contains(input);
}

bool PlayerInputManager::is_initiated(PlayerInput input) const {
  return current_initiations.contains(input);
}

void PlayerInputManager::engage(PlayerInput input) {
  current_inputs.insert(input);
}

void PlayerInputManager::disengage(PlayerInput input) {
  current_inputs.erase(input);
}

void PlayerInputManager::update() {
  current_initiations.clear();

  for (auto input : current_inputs) {
    if (!previous_tick_inputs.contains(input)) {
      current_initiations.insert(input);
    }
  }

  previous_tick_inputs = current_inputs;
}

}