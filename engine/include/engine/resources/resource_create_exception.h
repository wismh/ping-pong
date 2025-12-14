#pragma once
#include <stdexcept>

namespace engine {
    class ResourceCreateException final : public std::runtime_error {
    public:
        ResourceCreateException(
            const std::string& resourceType,
            const std::string& detail
        ) : std::runtime_error(fmt::format(
            "Failed to create resource [{}]: {}", resourceType, detail
        )) {

        }
    };
}
