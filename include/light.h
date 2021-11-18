#pragma once

#include <ext/glm/glm/glm.hpp>

#include <node.h>

enum class LightType { DIRECTIONAL, POINT };

class Light : public Node
{
public:
  Light() {}
  Light(glm::vec3 direction) {mDirection = direction;}

  float mConstant;
  float mLinear;
  float mQuadratic;

  glm::vec3 mAmbientColor;
  glm::vec3 mDiffuseColor;
  glm::vec3 mSpecularColor;

  glm::vec3 mDirection;

  LightType mType;
};
