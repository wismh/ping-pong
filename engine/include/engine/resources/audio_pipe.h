#pragma once
#include "engine/audio/audio_system.h"

#include "resource_pipe.h"

namespace engine {

class AudioPipe final : public ResourcePipe<Audio> {
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AudioSystem> _audioSystem;
public:
    explicit AudioPipe(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<AudioSystem>& audioSystem
    );

    std::shared_ptr<Audio> Load(const std::string& path, bool cache) override;
    void Unload(const std::string &path) override;
};

}
