#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace framework {

struct SDLDeleter {
    void operator()(SDL_Texture *texture_p) const {
      SDL_DestroyTexture(texture_p);
    }

    void operator()(SDL_Surface *surface_p) const {
      SDL_DestroySurface(surface_p);
    }

    void operator()(MIX_Track *track_p) const {
      MIX_DestroyTrack(track_p);
    }

    void operator()(MIX_Audio *audio_p) const {
      MIX_DestroyAudio(audio_p);
    }
};

SDLAssetManager::SDLAssetManager(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer)
    : renderer{renderer},
      mixer(std::move(mixer)) {};

void SDLAssetManager::load_image_png(std::string src_id, std::string path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (png_surface == nullptr) {
    throw std::runtime_error(std::format("Failed to create PNG surface '{}': {}", path, SDL_GetError()));
  }

  auto png_texture
      = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (png_texture == nullptr) {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface '{}': {}", path, SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({src_id, png_texture});
}

std::shared_ptr<SDL_Texture> SDLAssetManager::get_texture(const std::string &src_id) const {
  return textures.at(src_id);
}

void SDLAssetManager::load_audio_wav(std::string sound_id, std::string path) {
  auto audio_track = std::shared_ptr<MIX_Track>(MIX_CreateTrack(mixer.get()), SDLDeleter());
  if (audio_track == nullptr) {
    throw std::runtime_error(std::format("Couldn't create mix track '{}': {}", sound_id, SDL_GetError()));
  }

  auto audio = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer.get(), path.c_str(), true), SDLDeleter());
  if (audio == nullptr) {
    throw std::runtime_error(std::format("Failed to load audio '{}': {}", path, SDL_GetError()));
  }

  MIX_SetTrackAudio(audio_track.get(), audio.get());

  audio_tracks.insert({sound_id, audio_track});
}

void SDLAssetManager::play_sound(const std::string &sound_id) {
  if (audio_tracks.contains(sound_id)) {
    MIX_PlayTrack(audio_tracks.at(sound_id).get(), 0);
  }
}

void SDLAssetManager::stop_sound(const std::string &sound_id) {
  if (audio_tracks.contains(sound_id)) {
    MIX_StopTrack(audio_tracks.at(sound_id).get(), 0);
  }
}

void SDLAssetManager::clear_all() {
  textures.clear();
  audio_tracks.clear();
}

} // namespace framework