#pragma once

#include "framework/asset_loader.hpp"
#include "framework/audio_player.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace framework {

class SDLAssetManager : public AssetLoader, public AudioPlayer {
  private:
    // Renderer is cleaned up automatically by SDL, so it doesn't need to be a smart pointer.
    SDL_Renderer *renderer;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

    std::shared_ptr<MIX_Mixer> mixer;
    std::unordered_map<std::string, std::shared_ptr<MIX_Track>> audio_tracks;

    void create_texture_from_file_png(const std::string &path);
    void create_audio_track_from_file_wav(const std::string &path);

  public:
    SDLAssetManager(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer);

    void load_image_png(const std::string &path) override;
    void load_images_in_dir_png(const std::string &path) override;
    std::shared_ptr<SDL_Texture> get_texture(const std::string &filename) const;

    void load_audio_wav(const std::string &path) override;
    void load_audio_in_dir_wav(const std::string &path) override;
    void play_sound(const std::string &filename) override;
    void stop_sound(const std::string &filename) override;

    void clear_all();
};

} // namespace framework