#pragma once
#include "../utils.h"

namespace engine {

template <typename SrcT, typename DestT>
class ResourcePipe {
protected:
    std::unordered_map<std::string, std::shared_ptr<DestT>> _resources;
public:
    virtual ~ResourcePipe() = default;

    virtual std::shared_ptr<DestT> Load(const std::string& id, SrcT source) = 0;
    virtual void Unload(const std::string& id) = 0;

    void UnloadAll() {
        for (auto resource = _resources.begin(); resource != _resources.end(); ++resource)
            Unload(resource->first);
    }
};

}
