#pragma once
#include <vector>

namespace engine {

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

struct MeshDesc {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexes;
};

}
