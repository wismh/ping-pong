#include "engine/resources/audio_pipe.h"

namespace engine {

AudioPipe::AudioPipe(
    const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<AudioSystem>& audioSystem
) :
    _logger(logger->Get()),
    _audioSystem(audioSystem) {

}

std::shared_ptr<Audio> AudioPipe::Load(const std::string& path, const bool cache) {
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

void AudioPipe::Unload(const std::string &path) {
    if (!_cache.contains(path))
        return;

    _logger->info("Unloading audio '{}'", path);

    auto resource = _cache.at(path);
    resource.reset();

    _cache.erase(path);
}

}