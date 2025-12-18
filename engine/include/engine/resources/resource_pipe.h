#pragma once
#include "../utils.h"

namespace engine {

class IResourcePipe {
public:
    virtual ~IResourcePipe() = default;
    virtual void UnloadAll() = 0;
};

template <typename DestT>
class ResourcePipe : public IResourcePipe {
protected:
    std::unordered_map<std::string, std::shared_ptr<DestT>> _cache;
public:
    virtual std::shared_ptr<DestT> Load(const std::string& path, bool cache) = 0;
    virtual void Unload(const std::string& path) = 0;

    void UnloadAll() override {
        auto cache (_cache);
        for (auto resource = cache.begin(); resource != cache.end(); ++resource)
            Unload(resource->first);
    }
};

}
