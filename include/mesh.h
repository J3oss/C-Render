#pragma once

#include <vector>

#include <ext/glm/glm/glm.hpp>
#include <material.h>

#include <shaders/shader.h>

class Mesh : public Node {
public:
  std::vector<uint32_t> mIndices;
  std::vector<glm::vec4> mPositions;
  std::vector<glm::vec4> mNormals;
  std::vector<glm::vec4> mTangets;
  std::vector<glm::vec2> mUVs;

  Shader shader;
  std::shared_ptr<Material> mMaterial;
};
