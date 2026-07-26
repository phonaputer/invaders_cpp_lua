#pragma once

#include "framework/system.hpp"
#include <memory>

namespace framework {

class SystemRegistry {
  public:
    virtual ~SystemRegistry() = default;
    virtual void add_update_system(std::unique_ptr<System> system) = 0;
    virtual void add_draw_system(std::unique_ptr<System> system) = 0;
};

} // namespace framework