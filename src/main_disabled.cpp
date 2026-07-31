#include <lualib.h>

#include <LuaBridge/LuaBridge.h>

#include "framework/script_environment.hpp"
#include <entt.hpp>
#include <iostream>
#include <memory>
#include <string>

struct Position {
    float x;
    float y;
    float z;
};

struct Velocity {
    float x;
    float y;
};

void print_position(Position pos) {
  std::cout << "Position(x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << ")\n";
}

int varargs_function_raw_style(lua_State *L) {
  int numArgs = lua_gettop(L);

  for (int i = 1; i <= numArgs; i++) {
    luabridge::LuaRef ref = luabridge::LuaRef::fromStack(L, i);

    if (!ref.isString()) {
      std::cout << "Where is my RAW STYLE strang, mang\n";
    } else {
      std::cout << ref.cast<std::string>().value() << ", ";
    }
  }

  std::cout << "\n";

  return 0;
}

void varargs_function(luabridge::LuaRef ref) {
  if (!ref.isTable()) {
    std::cout << "I'm tabling this motion\n";
    return;
  }

  for (int i = 1; i <= ref.length(); i++) {
    auto elem = ref[i];

    if (!elem.isString()) {
      std::cout << "Where is my strang, mang\n";
    } else {
      std::cout << elem.cast<std::string>().value() << ", ";
    }
  }

  std::cout << "\n";
}

void run_callback(luabridge::LuaRef callback) {
  if (!callback.isFunction()) {
    std::cout << "what have you done, son?\n";
    return;
  }

  callback("good golly miss molly");
}

struct LuaStateDeleter {
    void operator()(lua_State *L) const {
      lua_close(L);
    }
};

int main() {
  entt::registry ecs;

  framework::ScriptEnvironment script_env{ecs};

  script_env.register_component<Velocity>("Velocity", [](auto &clazz) {
    clazz.addProperty("x", &Velocity::x, &Velocity::x).addProperty("y", &Velocity::y, &Velocity::y);
  });
  script_env.register_component<Position>("Position", [](auto &clazz) {
    clazz.addProperty("x", &Position::x, &Position::x)
        .addProperty("y", &Position::y, &Position::y)
        .addProperty("z", &Position::z, &Position::z);
  });

  script_env.register_function("printPosition", print_position);
  script_env.register_function("runCallback", print_position);
  script_env.register_function("printAll", print_position);
  script_env.register_function("printAllRAW", print_position);

  script_env.exec_all_script_files_in_dir("invasion");

  std::cout << "---Run 1---\n";
  script_env.call_function("doHelloWorld");
  auto result = script_env.call_function<lua_Number>("addEm", 2, 4);
  std::cout << "addEm: " << result << "\n";
  std::cout << "---Run 1---\n";

  std::cout << "\n\n=====Finished Lua Runs=====\n\n\n";

  auto view = ecs.view<Position>();

  for (auto [e, position] : view.each()) {
    std::cout << "---Entity " << entt::to_integral(e) << "---\n";
    print_position(position);
    std::cout << "---Entity " << entt::to_integral(e) << "---\n\n";
  }
}
