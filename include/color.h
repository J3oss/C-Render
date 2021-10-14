#pragma once

#include <ext/glm/glm/glm.hpp>

class Color
{
public:

  glm::vec3 mColor;

  Color()
  {
    mColor = glm::vec3(0,0,0);
  }

  Color(uint32_t R, uint32_t G, uint32_t B)
  {
    mColor = glm::vec3(R,G,B);
  }

  Color(glm::vec3 color)
  {
    mColor = glm::vec3(color);
  }
};
