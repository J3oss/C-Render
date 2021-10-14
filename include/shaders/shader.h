#pragma once

#include <ext/glm/glm/glm.hpp>

class Shader
{
public:
  glm::mat4 mvpMatrix;
  glm::mat4 modelMatrix;
  glm::mat4 normalMatrix;

  std::vector<glm::vec4> inPositions;
  std::vector<glm::vec4> inNormals;
  std::vector<glm::vec4> inTangents;
  std::vector<glm::vec2> inUVs;

  std::shared_ptr<Camera> inCamera;
  std::shared_ptr<Material> inMaterial;
  std::vector< std::shared_ptr<Light> > inLights;

  glm::vec4 outPositions[3];

  virtual void vertex(uint32_t index[3]) {};
  virtual Color fragment(glm::vec3 bc) { return Color(255,255,255); };

protected:
  glm::vec2 outUVs[3];
  glm::vec3 outNormals[3];
  glm::vec3 outTangents[3];
  glm::vec3 outBitangents[3];
  glm::mat3 outTbnMatrix[3];
};
