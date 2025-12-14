#pragma once
#include <SDL3_mixer/SDL_mixer.h>

namespace engine {

class Audio {
    friend class AudioEventsManager;
    MIX_Audio* _audio;
public:
    explicit Audio(MIX_Audio* audio) :
        _audio(audio) {
    }

    ~Audio() {
        MIX_DestroyAudio(_audio);
    }
};

}