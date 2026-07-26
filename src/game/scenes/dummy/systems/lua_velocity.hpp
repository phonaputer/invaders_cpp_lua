#pragma once

#include "framework/system.hpp"

namespace systems {

class LuaVelocity : public framework::System {
  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems