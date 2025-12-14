#include "engine/resources/shader_pipe.h"
#include "engine/resources/shader_desc.h"

#include "tinyxml2.h"

namespace engine {

ShaderPipe::ShaderPipe(
    const std::shared_ptr<Logger>& logger,
    const std::shared_ptr<render::IGraphicFabric>& fabric
) :
    _logger(logger->Get()),
    _fabric(fabric) {

}

std::shared_ptr<render::IShader> ShaderPipe::Load(const std::string& path, const bool cache) {
    if (_cache.contains(path))
        return _cache[path];

    _logger->info("Loading shader from '{}'", path);

    using namespace tinyxml2;

    XMLDocument doc;

    if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
        _logger->error("Failed to load shader from '{}'", path);
        return nullptr;
    }

    auto* root = doc.RootElement();
    if (!root) {
        _logger->error("Shader has no XML root element in '{}'", path);
        return nullptr;
    }

    auto* v = root->FirstChildElement("vertex");
    if (!v || !v->GetText()) {
        _logger->error("Vertex shader missing in '{}'", path);
        return nullptr;
    }
    const auto& vertex = v->GetText();

    auto* f = root->FirstChildElement("fragment");
    if (!f || !f->GetText()) {
        _logger->error("Fragment shader missing in '{}'", path);
        return nullptr;
    }
    const auto& fragment = f->GetText();

    const ShaderDesc desc{
        .vertexShaderSrc = vertex,
        .fragmentShaderSrc = fragment
    };
    std::shared_ptr<render::IShader> shader;
    std::string errorMessage;

    if (!_fabric->TryCreateShader(desc, shader, errorMessage)) {
        _logger->error(errorMessage);
        return nullptr;
    }

    _logger->debug("Loaded shader successfully from '{}'", path);

    if (cache)
        _cache.insert({path, shader});
    return shader;
}

void ShaderPipe::Unload(const std::string &path) {
    if (!_cache.contains(path))
        return;

    _logger->info("Unloading shader '{}'", path);

    auto resource = _cache.at(path);
    resource.reset();

    _cache.erase(path);
}

}