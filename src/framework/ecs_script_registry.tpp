#include "framework/ecs_script_registry.hpp"

#include <string>

#include <LuaBridge/LuaBridge.h>

namespace framework {

constexpr std::string TYPE_ID_PROPERTY_NAME = "TypeID";

inline ECSScriptRegistry::ECSScriptRegistry(entt::registry &ecs, lua_State &L)
    : ecs{ecs},
      L{L} {
  // clang-format off
  luabridge::getGlobalNamespace(&L)
    .beginNamespace("ECS")
      .addFunction(
        "create", 
        [this]() { return create_entity(); }
      )
      .addFunction(
        "view", 
        [this](luabridge::LuaRef table) -> luabridge::LuaRef { return generate_view_callback(table); }
      )
    .endNamespace();
  // clang-format on
}

template <typename T, typename F>
void ECSScriptRegistry::register_component(std::string name, F &&fields_register_func) {
  auto type_id = cur_type_id++;

  // clang-format off
  luabridge::getGlobalNamespace(&L)
      .beginNamespace("ECS")
      .addFunction(
          ("set" + name).c_str(),
          [this](uint32_t e_int, T t) { 
            ecs.emplace_or_replace<T>(static_cast<entt::entity>(e_int), t); 
          }
      )
      .addFunction(
          ("has" + name).c_str(), 
          [this](uint32_t e_int) { 
            return ecs.all_of<T>(static_cast<entt::entity>(e_int)); 
          }
      )
      .addFunction(
          ("get" + name).c_str(), 
          [this](uint32_t e_int) { 
            return ecs.get<T>(static_cast<entt::entity>(e_int)); 
          }
      )
      .addFunction(
          ("remove" + name).c_str(), 
          [this](uint32_t e_int) { 
            ecs.remove<T>(static_cast<entt::entity>(e_int)); 
          }
      )
      .endNamespace();
  // clang-format on

  auto clazz = std::move(
      luabridge::getGlobalNamespace(&L)
          .beginNamespace("Components")
          .beginClass<T>(name.c_str())
          .template addConstructor<void()>()
          .addStaticProperty(TYPE_ID_PROPERTY_NAME.c_str(), [type_id]() -> uint32_t { return type_id; })
  );

  std::forward<F>(fields_register_func)(clazz);

  clazz.endClass().endNamespace();

  type_id_to_view_func.insert({
      type_id,
      [](entt::registry &view_ecs, entt::runtime_view &view) { view.iterate(view_ecs.storage<T>()); },
  });
}

uint32_t ECSScriptRegistry::create_entity() {
  return entt::to_integral(ecs.create());
}

inline luabridge::LuaRef ECSScriptRegistry::generate_view_callback(luabridge::LuaRef table) {
  if (!table.isTable()) {
    // TODO - decide a better way to handle these errors
    throw std::runtime_error("Need a table, did not get one");
  }

  entt::runtime_view view{};

  for (int i = 1; i <= table.length(); i++) {
    auto elem = table[i];
    if (!elem.isTable()) {
      throw std::runtime_error("Need a object table, did not get one");
    }

    auto cast_result = elem[TYPE_ID_PROPERTY_NAME.c_str()].cast<uint32_t>();
    if (!cast_result.error()) {
      type_id_to_view_func.at(cast_result.value())(ecs, view);
    }
  }

  return luabridge::newFunction(&L, [view](luabridge::LuaRef callback) {
    if (!callback.isFunction()) {
      throw std::runtime_error("Need a function, did not get one");
      return;
    }

    for (auto entity : view) {
      callback(entt::to_integral(entity));
    }
  });
}

} // namespace framework