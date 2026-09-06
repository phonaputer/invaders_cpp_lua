#pragma once

#include <cstdint>

namespace framework {

// This will intentionally wrap around when it hits uint16 max (65,535).
// This allows setting events to happen <= 65,535 ticks in the future, but no further.
using Tick = uint16_t;

}