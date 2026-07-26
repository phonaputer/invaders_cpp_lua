#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

namespace framework {

struct SDLDeleter {
    void operator()(SDL_Texture *p) const {
      SDL_DestroyTexture(p);
    }

    void operator()(SDL_Surface *p) const {
      SDL_DestroySurface(p);
    }

    void operator()(MIX_Track *p) const {
      MIX_DestroyTrack(p);
    }

    void operator()(MIX_Audio *p) const {
      MIX_DestroyAudio(p);
    }
};

SDLAssetManager::SDLAssetManager(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer)
    : renderer{renderer},
      mixer{mixer} {};

void SDLAssetManager::load_image_png(std::string src_id, std::string path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (!png_surface.get()) {
    throw std::runtime_error(std::format("Failed to create PNG surface '{}': {}", path, SDL_GetError()));
  }

  auto png_texture
      = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (!png_texture.get()) {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface '{}': {}", path, SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({src_id, png_texture});
}

std::shared_ptr<SDL_Texture> SDLAssetManager::get_texture(std::string src_id) const {
  return textures.at(src_id);
}

void SDLAssetManager::load_audio_wav(std::string sound_id, std::string path) {
  auto audio_track = std::shared_ptr<MIX_Track>(MIX_CreateTrack(mixer.get()), SDLDeleter());
  if (!audio_track.get()) {
    throw std::runtime_error(std::format("Couldn't create mix track '{}': {}", sound_id, SDL_GetError()));
  }

  auto audio = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer.get(), path.c_str(), true), SDLDeleter());
  if (!audio.get()) {
    throw std::runtime_error(std::format("Failed to load audio '{}': {}", path, SDL_GetError()));
  }

  MIX_SetTrackAudio(audio_track.get(), audio.get());

  audio_tracks.insert({sound_id, audio_track});
}

void SDLAssetManager::play_sound(std::string sound_id) {
  if (audio_tracks.contains(sound_id)) {
    MIX_PlayTrack(audio_tracks.at(sound_id).get(), 0);
  }
}

void SDLAssetManager::stop_sound(std::string sound_id) {
  if (audio_tracks.contains(sound_id)) {
    MIX_StopTrack(audio_tracks.at(sound_id).get(), 0);
  }
}

void SDLAssetManager::clear_all() {
  textures.clear();
  audio_tracks.clear();
}

} // namespace framework