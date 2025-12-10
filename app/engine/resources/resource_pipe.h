#pragma once
#include "../utils.h"

namespace engine {

class IResourcePipe {
public:
    virtual ~IResourcePipe() = default;
};

template <typename DestT>
class ResourcePipe : public IResourcePipe {
protected:
    std::unordered_map<std::string, std::shared_ptr<DestT>> _cache;
public:
    virtual std::shared_ptr<DestT> Load(const std::string& path, bool cache) = 0;
    virtual void Unload(const std::string& path) = 0;

    void UnloadAll() {
        for (auto resource = _cache.begin(); resource != _cache.end(); ++resource)
            Unload(resource->first);
    }
};

}
