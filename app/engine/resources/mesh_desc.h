#pragma once
#include <vector>

namespace engine {

struct MeshDesc {
    std::vector<float> vertices;
    std::vector<uint32_t> indexes;
};

}
