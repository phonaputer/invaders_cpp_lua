#pragma once

#include "framework/script_environment.hpp"
#include "game/scenes/invasion/infra/callback_registry.hpp"
#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace infra {

using WeaponID = uint8_t;

struct Weapon {
    uint16_t ticks_per_shot;
    CallbackID shoot_callback;
};

class WeaponGetter {
  public:
    WeaponGetter() = default;
    virtual ~WeaponGetter() = default;
    WeaponGetter(const WeaponGetter &) = delete;
    WeaponGetter &operator=(const WeaponGetter &) = delete;
    WeaponGetter(WeaponGetter &&) = delete;
    WeaponGetter &operator=(WeaponGetter &&) = delete;

    [[nodiscard]] virtual std::optional<Weapon> get_weapon(WeaponID id) const = 0;
};

class WeaponRegistry : public WeaponGetter {
  private:
    WeaponID cur_id = 0;
    std::mutex cur_id_lock;
    std::unordered_map<WeaponID, Weapon> weapons;

    WeaponID get_next_id();

  public:
    WeaponID register_weapon(Weapon weapon);
    [[nodiscard]] std::optional<Weapon> get_weapon(WeaponID id) const override;
};

void add_callback_registry_to_script_env(framework::ScriptEnvironment &scripts, WeaponRegistry &weapons);

} // namespace infra