#pragma once
#include <shaders/shader.h>
#include <ext/glm/glm/gtx/scalar_multiplication.hpp>

class PhongShader : public Shader
{
public:
  virtual void vertex(uint32_t index[3])
  {
    for (uint32_t i = 0; i < 3; i++)
    {
      positions[i] = modelMatrix * inPositions[index[i]];
      outPositions[i] = mvpMatrix * inPositions[index[i]];

      outUVs[i] = inUVs[index[i]];
      outNormals[i] = normalMatrix * inNormals[index[i]];
    }
  }

  virtual Color fragment(glm::vec3 bc)
  {
    glm::vec2 uv = outUVs[0] * bc[0] + outUVs[1] * bc[1] + outUVs[2] * bc[2];
    glm::vec3 position = positions[0] * bc[0] + positions[1] * bc[1] + positions[2] * bc[2];
    glm::vec3 normal = glm::normalize(outNormals[0] * bc[0] + outNormals[1] * bc[1] + outNormals[2] * bc[2]);

    glm::vec3 albedo = inMaterial->mAlbedoTexture->value(uv.x,uv.y).mColor;

    glm::vec3 light(0.0f);
    float attenuationFactor;
    glm::vec3 viewDirection;
    glm::vec3 lightDirection;
    for (size_t l = 0; l < inLights.size(); l++)
    {
      viewDirection = glm::normalize(inCamera->GetPosition() - position);
      lightDirection = glm::normalize(inLights[l]->GetPosition() - position);

      float distance = glm::distance(inLights[l]->GetPosition(), position);
      attenuationFactor = 1 / ( inLights[l]->mConstant +
        inLights[l]->mLinear * distance + inLights[l]->mQuadratic * distance * distance);

      switch (inLights[l]->mType)
      {
        case LightType::POINT:
        light += calculatePointLight(inLights[l].get(), lightDirection, viewDirection, normal, attenuationFactor);
        break;
      }
    }
    light = glm::clamp(light, 0.0f, 1.0f);

    Color final(light * albedo);

    return final;
  }

private:
    glm::vec3 positions[3];

    glm::vec3 calculatePointLight(const Light* light, glm::vec3 lightDirection, glm::vec3 viewDirection, glm::vec3 normal, float attenuationFactor)
    {
      glm::vec3 ambient(0.0f);
      glm::vec3 diffuse(0.0f);
      glm::vec3 specular(0.0f);

      ambient = 0.1f * light->mAmbientColor;

      float dFactor = std::max(glm::dot(normal, lightDirection), 0.0f);
      diffuse = (dFactor * light->mDiffuseColor);

      glm::vec3 reflectDir = (-1*lightDirection) - 2.f * glm::dot((-1* lightDirection), normal) * normal;
      float sFactor = pow(std::max(glm::dot(viewDirection, reflectDir), 0.0f), 32);
      specular = 0.5f * sFactor * light->mSpecularColor;

      return (ambient + diffuse + specular) * attenuationFactor;
    }
};
