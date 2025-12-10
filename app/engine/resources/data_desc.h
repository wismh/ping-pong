#pragma once

namespace engine {

struct DataDesc {
  std::unique_ptr<unsigned char[]> data;
  size_t size;
};

}