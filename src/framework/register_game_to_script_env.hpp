#pragma once

#include "framework/game.hpp"
#include "framework/script_environment.hpp"

namespace framework {

void register_game_to_script_env(Game &game, ScriptEnvironment &scripts);

}