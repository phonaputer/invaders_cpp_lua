#pragma once

#include "framework/system.hpp"
#include <memory>

namespace framework {

class SystemRegistry {
  public:
    SystemRegistry() = default;
    virtual ~SystemRegistry() = default;
    SystemRegistry(const SystemRegistry &) = delete;
    SystemRegistry &operator=(const SystemRegistry &) = delete;
    SystemRegistry(SystemRegistry &&) = delete;
    SystemRegistry &operator=(SystemRegistry &&) = delete;

    virtual void add_update_system(std::unique_ptr<System> system) = 0;
    virtual void add_draw_system(std::unique_ptr<System> system) = 0;
};

} // namespace framework