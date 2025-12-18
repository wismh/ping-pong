#pragma once
#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "mesh_pipe.h"
#include "resource_pipe.h"
#include "shader_pipe.h"
#include "texture_pipe.h"
#include "ui_image_pipe.h"
#include "audio_pipe.h"

namespace engine {

class AssetsDb final {
    std::unordered_map<std::type_index, std::unique_ptr<IResourcePipe>> _pipes;
public:

    AssetsDb(
        const std::shared_ptr<Logger>& logger,
        const std::shared_ptr<render::IGraphicFactory>& fabric,
        const std::shared_ptr<ui::UICanvas>& canvas,
        const std::shared_ptr<AudioSystem>& audioSystem
    ) {
        RegisterPipe<render::IMesh, MeshPipe>(logger, fabric);
        RegisterPipe<render::ITexture, TexturePipe>(logger, fabric);
        RegisterPipe<render::IShader, ShaderPipe>(logger, fabric);
        RegisterPipe<UIImage, UIImagePipe>(logger, canvas);
        RegisterPipe<Audio, AudioPipe>(logger, audioSystem);
    }

    void Dispose() {
        for (auto& pipe : _pipes)
            pipe.second->UnloadAll();
    }


    static std::string GetAssetPath(const char* localPath) {
        return std::string(ASSETS_PATH) + "/" + localPath;
    }

    template <typename T>
    std::shared_ptr<T> Get(const std::string& localPath, bool cache = true) {
        auto& pipe = GetPipe<T>();
        auto path = GetAssetPath(localPath.c_str());
        return pipe.Load(path, cache);
    }
private:
    template<typename T, typename PipeT, typename... Args>
    void RegisterPipe(Args&&... args) {
        auto typeId = std::type_index(typeid(T));
        _pipes[typeId] = std::make_unique<PipeT>(std::forward<Args>(args)...);
    }

    template<typename T>
    ResourcePipe<T>& GetPipe() {
        const auto typeId = std::type_index(typeid(T));

        const auto it = _pipes.find(typeId);
        if (it == _pipes.end())
            throw std::runtime_error("Pipe for this type is not registered!");

        return *static_cast<ResourcePipe<T>*>(it->second.get());
    }
};

}
