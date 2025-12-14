#pragma once
#include "resource_pipe.h"
#include "../audio/audio_system.h"

namespace engine {

class AudioPipe final : public ResourcePipe<Audio> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AudioSystem> _audioSystem;
public:
    explicit AudioPipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<AudioSystem>& audioSystem
    ) :
        _audioSystem(audioSystem),
        _logger(logger->Get()) {

    }

    std::shared_ptr<Audio> Load(const std::string& path, const bool cache) override {
        if (_cache.contains(path))
            return _cache[path];

        _logger->info("Loading audio from '{}'", path);

        auto audioMix = MIX_LoadAudio(_audioSystem->GetMixer(), path.c_str(), false);
        if (audioMix == nullptr) {
            _logger->error("Failed to load audio from '{}'", path);
            return nullptr;
        }

        auto audio = std::make_shared<Audio>(audioMix);

        _logger->debug("Loaded audio successfully from '{}'", path);

        if (cache)
            _cache.insert({path, audio});
        return audio;
    }

    void Unload(const std::string &path) override {
        if (!_cache.contains(path))
            return;

        _logger->info("Unloading audio '{}'", path);

        auto resource = _cache.at(path);
        resource.reset();

        _cache.erase(path);
    }
};

}
