#pragma once
#include "../utils.h"
#include "audio.h"

namespace engine {

class AudioSystem {
    std::shared_ptr<spdlog::logger> _logger;
    MIX_Mixer* _mixer = nullptr;
    bool _initialized = false;
public:
    explicit AudioSystem(const std::shared_ptr<Logger>& logger) :
        _logger(logger->Get()) {

    }

    ~AudioSystem() {
        Dispose();
    }

    bool Init() {
        _logger->info("Initializing audio system...");

        if (!MIX_Init()) {
            _logger->error("Failed to initialize audio system: {}", SDL_GetError());
            return false;
        }

        _mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!_mixer) {
            _logger->error("Failed to initialize audio system: {}", SDL_GetError());
            return false;
        }

        _logger->debug("Initialized audio system successfully");

        _initialized = true;
        return true;
    }


    void Dispose() {
        if (!_initialized)
            return;

        _logger->debug("Disposing audio system...");

        MIX_DestroyMixer(_mixer);
        MIX_Quit();
        _initialized = false;

        _logger->info("Disposed audio system successfully");
    }

    MIX_Mixer* GetMixer() const {
        return _mixer;
    }
};

}