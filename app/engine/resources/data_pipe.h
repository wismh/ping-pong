#pragma once
#include <fstream>

#include "data_desc.h"
#include "resource_pipe.h"

namespace engine {

    class DataPipe final : public ResourcePipe<const char*, DataDesc> {
        std::shared_ptr<spdlog::logger> _logger;
    public:
        explicit DataPipe(
            const std::shared_ptr<Logger>& logger
        ) : _logger(logger->Get()) {

        }

        std::shared_ptr<DataDesc> Load(const std::string &id, const char* source) override {
            auto dataDesc = std::make_shared<DataDesc>();

            std::ifstream file(source, std::ios::binary | std::ios::ate);
            if (!file) {
                _logger->error("Failed to open file: {}", source);
                return nullptr;
            }

            const std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            dataDesc->size = static_cast<size_t>(fileSize);
            dataDesc->data = std::make_unique<unsigned char[]>(dataDesc->size);

            if (!file.read(reinterpret_cast<char*>(dataDesc->data.get()), fileSize)) {
               _logger->error("Failed to read file: {}", source);
                return nullptr;
            }

            _resources.insert({id, dataDesc});
            return dataDesc;
        }

        void Unload(const std::string &id) override {
            if (!_resources.contains(id))
                return;

            auto resource = _resources.at(id);
            resource->data.release();

            _resources.erase(id);
        }
    };

}
