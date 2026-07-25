include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.4.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
  SDL3_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG        release-3.2.4
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)

FetchContent_Declare(
  Luau
  GIT_REPOSITORY https://github.com/luau-lang/luau.git
  GIT_TAG        0.730
  GIT_SHALLOW    TRUE
  SYSTEM
)
FetchContent_MakeAvailable(Luau)

FetchContent_Declare(
  LuaBridge3
  GIT_REPOSITORY https://github.com/kunitoki/LuaBridge3.git
  GIT_TAG        3.0-rc12
  GIT_SHALLOW    TRUE
  SYSTEM
)
FetchContent_MakeAvailable(LuaBridge3)

add_library(vendor_lib INTERFACE)
target_link_libraries(
  vendor_lib INTERFACE 
    SDL3::SDL3 
    SDL3_mixer::SDL3_mixer 
    Luau.Compiler 
    Luau.VM
    LuaBridge
)