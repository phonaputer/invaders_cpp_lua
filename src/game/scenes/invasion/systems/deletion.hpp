#pragma once

#include "framework/system.hpp"

namespace systems {

class Deletion : public framework::System {
  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems