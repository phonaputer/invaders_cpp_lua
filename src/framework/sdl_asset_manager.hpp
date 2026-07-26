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

  public:
    SDLAssetManager(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer);

    void load_image_png(std::string src_id, std::string path) override;
    std::shared_ptr<SDL_Texture> get_texture(std::string src_id) const;

    void load_audio_wav(std::string sound_id, std::string path) override;
    void play_sound(std::string sound_id) override;
    void stop_sound(std::string sound_id) override;

    void clear_all();
};

} // namespace framework