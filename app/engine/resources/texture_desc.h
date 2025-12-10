#pragma once

namespace engine {

struct TextureDesc {
    int width, height;
    int channels;
    unsigned char* data;
};

}