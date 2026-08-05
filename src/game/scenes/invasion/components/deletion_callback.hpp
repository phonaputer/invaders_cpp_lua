#pragma once

#include <string>

namespace components {

struct DeletionCallback {
    std::string package;
    std::string callback;
};

} // namespace components