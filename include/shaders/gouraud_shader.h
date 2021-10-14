#pragma once
#include <shaders/shader.h>
#include <ext/glm/glm/gtx/scalar_multiplication.hpp>

class GouraudShader : public Shader
{
public:
  virtual void vertex(uint32_t index[3])
  {
    for (uint32_t i = 0; i < 3; i++)
    {
      outPositions[i] = mvpMatrix * inPositions[index[i]];
      outUVs[i] = inUVs[index[i]];
      outNormals[i] = normalMatrix * inNormals[index[i]];

      float intensity = 0.2f, tIntensity;
      for (size_t l = 0; l < inLights.size(); l++)
      {
        tIntensity = (10 * glm::dot(outNormals[i], glm::normalize(inLights[l]->mDirection)));
        tIntensity = CLAMP(tIntensity, 0.0f, 1.0f);
        intensity += tIntensity;
      }
      intensity = CLAMP(intensity, 0.0f, 1.0f);

      colors[i] = intensity * inMaterial->mAlbedoTexture->value(outUVs[i].x,outUVs[i].y).mColor;
    }
  }

  virtual Color fragment(glm::vec3 bc)
  {
    glm::vec3 interpolatedColor = colors[0] * bc[0] + colors[1] * bc[1] + colors[2] * bc[2];
    return Color(interpolatedColor);
  }

private:
  glm::vec3 colors[3];
};
