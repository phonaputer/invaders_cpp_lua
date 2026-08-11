#include "framework/sdl_asset_manager.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace framework {

const std::string_view IMAGES_DIRECTORY_PREFIX = "./assets/images/";
const std::string_view AUDIO_DIRECTORY_PREFIX = "./assets/audio/";

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

void SDLAssetManager::load_image_png(const std::string &path) {
  create_texture_from_file_png(std::string(IMAGES_DIRECTORY_PREFIX) + path);
}

void SDLAssetManager::load_images_in_dir_png(const std::string &path) {
  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(std::string(IMAGES_DIRECTORY_PREFIX) + path)) {
      create_texture_from_file_png(entry.path().string());
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "SDLAssetManager: Failed to open image directory '" << path << "': " << e.what() << "\n";
    assert(false && "Failed to open directory");
  }
}

std::shared_ptr<SDL_Texture> SDLAssetManager::get_texture(const std::string &filename) const {
  // Potential improvement - check if texture exists and return a fallback texture if not
  return textures.at(filename);
}

void SDLAssetManager::load_audio_wav(const std::string &path) {
  create_audio_track_from_file_wav(std::string(AUDIO_DIRECTORY_PREFIX) + path);
}

void SDLAssetManager::load_audio_in_dir_wav(const std::string &path) {
  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(std::string(AUDIO_DIRECTORY_PREFIX) + path)) {
      create_audio_track_from_file_wav(entry.path().string());
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "SDLAssetManager: Failed to open audio directory '" << path << "': " << e.what() << "\n";
    assert(false && "Failed to open directory");
  }
}

void SDLAssetManager::play_sound(const std::string &filename) {
  if (audio_tracks.contains(filename)) {
    MIX_PlayTrack(audio_tracks.at(filename).get(), 0);
  }
}

void SDLAssetManager::stop_sound(const std::string &filename) {
  if (audio_tracks.contains(filename)) {
    MIX_StopTrack(audio_tracks.at(filename).get(), 0);
  }
}

void SDLAssetManager::clear_all() {
  textures.clear();
  audio_tracks.clear();
}

void SDLAssetManager::create_texture_from_file_png(const std::string &path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (png_surface == nullptr) {
    std::cerr << "SDLAssetManager: Failed to create PNG surface '" << path << "': " << SDL_GetError() << "\n";
    assert(false && "Failed to create PNG surface");
    return;
  }

  auto png_texture
      = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (png_texture == nullptr) {
    std::cerr << "SDLAssetManager: Failed to create PNG texture from surface'" << path
              << "': " << SDL_GetError() << "\n";
    assert(false && "Failed to create PNG texture from surface");
    return;
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  const std::string filename = std::filesystem::path(path).filename();

  textures.insert({filename, png_texture});
}

void SDLAssetManager::create_audio_track_from_file_wav(const std::string &path) {
  auto audio_track = std::shared_ptr<MIX_Track>(MIX_CreateTrack(mixer.get()), SDLDeleter());
  if (audio_track == nullptr) {
    std::cerr << "SDLAssetManager: Failed to create mix track '" << path << "': " << SDL_GetError() << "\n";
    assert(false && "Failed to create mix track");
    return;
  }

  auto audio = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer.get(), path.c_str(), true), SDLDeleter());
  if (audio == nullptr) {
    std::cerr << "SDLAssetManager: Failed to load audio '" << path << "': " << SDL_GetError() << "\n";
    assert(false && "Failed to load audio");
    return;
  }

  MIX_SetTrackAudio(audio_track.get(), audio.get());

  const std::string filename = std::filesystem::path(path).filename();

  audio_tracks.insert({filename, audio_track});
}

} // namespace framework