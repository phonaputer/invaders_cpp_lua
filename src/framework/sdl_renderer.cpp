#include "framework/sdl_renderer.hpp"
#include "framework/renderer.hpp"
#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <utility>

namespace framework {

SDLRenderer::SDLRenderer(SDL_Renderer *renderer, std::shared_ptr<SDLAssetManager> asset_manager)
    : renderer{renderer},
      asset_manager(std::move(asset_manager)) {
}

void SDLRenderer::draw_image(const DrawImageParams &params) {
  auto texture = asset_manager->get_texture(params.src_id);

  auto src_rect = SDL_FRect{
      .x = params.src_x + 0.02F,
      .y = params.src_y + 0.02F,
      .w = params.src_width - 0.04F,
      .h = params.src_height - 0.04F,
  };
  auto dest_rect = SDL_FRect{
      .x = params.dst_x,
      .y = params.dst_y,
      .w = params.dst_width,
      .h = params.dst_height,
  };

  SDL_RenderTexture(renderer, texture.get(), &src_rect, &dest_rect);
}

void SDLRenderer::draw_rect(const DrawRectParams &params) {
  auto sdl_rect = SDL_FRect{.x = params.x, .y = params.y, .w = params.width, .h = params.height};

  SDL_SetRenderDrawColor(renderer, params.r, params.g, params.b, 255);
  SDL_RenderRect(renderer, &sdl_rect);
}

void SDLRenderer::draw_line(const DrawLineParams &params) {
  SDL_SetRenderDrawColor(renderer, params.r, params.g, params.b, 255);
  SDL_RenderLine(renderer, params.start_x, params.start_y, params.end_x, params.end_y);
}

} // namespace framework