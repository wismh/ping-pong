#pragma once
#include "engine/core/event_bus.h"
#include "engine/resources/assetsdb.h"
#include "engine/utils.h"

#include "audio.h"
#include "audio_system.h"

namespace engine {

struct PlayMusicEvent {
    std::string name;
};

struct PlaySoundEvent {
    std::string name;
};

class AudioEventsManager {
    std::shared_ptr<AudioSystem> _audioSystem;
    std::shared_ptr<spdlog::logger> _logger;
    std::shared_ptr<AssetsDb> _assetsDb;

    MIX_Track* _track = nullptr;
    bool _initialized = false;
public:
    AudioEventsManager(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<EventBus>& bus,
        const std::shared_ptr<AudioSystem>& audioSystem,
        const std::shared_ptr<AssetsDb>& assetsDb
    ) :
        _audioSystem(audioSystem),
        _logger(logger->Get()),
        _assetsDb(assetsDb)
    {
        bus->Subscribe<PlayMusicEvent>(
            [this](auto& e) {
                PlayMusic(
                    _assetsDb->Get<Audio>(e.name)
                );
            }
        );

        bus->Subscribe<PlaySoundEvent>(
            [this](auto& e) {
                PlayAudio(
                    _assetsDb->Get<Audio>(e.name)
                );
            }
        );
    }

    void PlayAudio(const std::shared_ptr<Audio>& audio) {
        MIX_PlayAudio(_audioSystem->GetMixer(), audio->_audio);
    }

    void PlayMusic(const std::shared_ptr<Audio>& audio) {
        SDL_PropertiesID id = SDL_CreateProperties();
        SDL_SetNumberProperty(id, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

        MIX_SetTrackAudio(_track, audio->_audio);
        MIX_PlayTrack(_track, id);
    }

    bool Init() {
        _track = MIX_CreateTrack(_audioSystem->GetMixer());
        if (!_track) {
            _logger->error("Failed to initialize audio events manager: {}", SDL_GetError());
            return false;
        }

        _initialized = true;
        return true;
    }

    void Dispose() {
        _logger->info("Disposing audio events manager...");

        MIX_DestroyTrack(_track);

        _initialized = false;
    }
};

}