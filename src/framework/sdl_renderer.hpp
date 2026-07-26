#pragma once

#include "framework/renderer.hpp"
#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <string>

namespace framework {

class SDLRenderer : public Renderer {
  private:
    // Renderer is owned and cleaned up by SDL.
    SDL_Renderer *renderer;
    std::shared_ptr<SDLAssetManager> asset_manager;

  public:
    SDLRenderer(SDL_Renderer *renderer, std::shared_ptr<SDLAssetManager> asset_manager);
    void draw_image(const DrawImageParams &params) override;
    void draw_rect(const DrawRectParams &params) override;
    void draw_line(const DrawLineParams &params) override;
};

} // namespace framework