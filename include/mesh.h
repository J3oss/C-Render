#pragma once

#include <vector>
#include <point.h>

#include <ext/glm/glm/glm.hpp>

class Mesh {
public:
  std::vector<uint32_t> mIndices;
  std::vector<glm::vec4> mPositions;
};
