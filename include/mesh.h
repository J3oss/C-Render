#pragma once

#include <vector>

#include <ext/glm/glm/glm.hpp>

class Mesh : public Node {
public:
  std::vector<uint32_t> mIndices;
  std::vector<glm::vec4> mPositions;
};
