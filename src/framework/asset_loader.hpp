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

    virtual void load_image_png(const std::string &path) = 0;
    virtual void load_images_in_dir_png(const std::string &path) = 0;

    virtual void load_audio_wav(const std::string &path) = 0;
    virtual void load_audio_in_dir_wav(const std::string &path) = 0;
};

} // namespace framework