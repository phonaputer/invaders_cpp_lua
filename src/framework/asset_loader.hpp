#pragma once

#include <string>

namespace framework {

class AssetLoader {
  public:
    AssetLoader() = default;
    virtual ~AssetLoader() = default;
    AssetLoader(const AssetLoader &) = delete;
    AssetLoader &operator=(const AssetLoader &) = delete;
    AssetLoader(AssetLoader &&) = delete;
    AssetLoader &operator=(AssetLoader &&) = delete;

    virtual void load_image_png(std::string src_id, std::string path) = 0;
    virtual void load_audio_wav(std::string sound_id, std::string path) = 0;
};

} // namespace framework