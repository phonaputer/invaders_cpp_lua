#pragma once

#include "framework/script_environment.hpp"
#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace infra {

using CallbackID = uint16_t;

struct Callback {
    std::string package;
    std::string function;
};

class CallbackGetter {
  public:
    CallbackGetter() = default;
    virtual ~CallbackGetter() = default;
    CallbackGetter(const CallbackGetter &) = delete;
    CallbackGetter &operator=(const CallbackGetter &) = delete;
    CallbackGetter(CallbackGetter &&) = delete;
    CallbackGetter &operator=(CallbackGetter &&) = delete;

    [[nodiscard]] virtual std::optional<Callback> get_callback(CallbackID id) const = 0;
};

class CallbackRegistry : public CallbackGetter {
  private:
    CallbackID cur_id = 0;
    std::mutex cur_id_lock;
    std::unordered_map<CallbackID, Callback> callbacks;

    CallbackID get_next_id();

  public:
    CallbackID register_callback(const std::string &package, const std::string &function);
    [[nodiscard]] std::optional<Callback> get_callback(CallbackID id) const override;
};

void add_callback_registry_to_script_env(framework::ScriptEnvironment &scripts, CallbackRegistry &callbacks);

} // namespace infra