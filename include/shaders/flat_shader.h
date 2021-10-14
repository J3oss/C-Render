#pragma once
#include <shaders/shader.h>
#include <ext/glm/glm/gtx/scalar_multiplication.hpp>

class FlatShader : public Shader
{
public:
  virtual void vertex(uint32_t index[3])
  {
    glm::vec2 uv;
    glm::vec3 normal;

    for (uint32_t i = 0; i < 3; i++)
    {
      outPositions[i] = mvpMatrix * inPositions[index[i]];
      outUVs[i] = inUVs[index[i]];
      outNormals[i] = normalMatrix * inNormals[index[i]];

      uv     = (outUVs[0] + outUVs[1] + outUVs[2]) / 3;
      normal = (outNormals[0] + outNormals[1] + outNormals[2]) / 3;
    }

    float intensity = 0.2f, tIntensity;
    for (size_t l = 0; l < inLights.size(); l++)
    {
      tIntensity = (10 * glm::dot(normal, glm::normalize(inLights[l]->mDirection)));
      tIntensity = CLAMP(tIntensity, 0.0f, 1.0f);
      intensity += tIntensity;
    }
    intensity = CLAMP(intensity, 0.0f, 1.0f);

    outColor = intensity * inMaterial->mAlbedoTexture->value(uv.x,uv.y).mColor;
  }

  virtual Color fragment(glm::vec3 bc)
  {
    return Color(outColor);
  }
private:
   glm::vec3 outColor;
};
