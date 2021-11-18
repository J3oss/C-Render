#pragma once
#include <shaders/shader.h>

const float PI = 3.14159265359;

class PBRShader : public Shader
{
public:
  virtual void vertex(uint32_t index[3])
  {
    glm::vec3 tangents[3], bitangents[3];
    for (uint32_t i = 0; i < 3; i++)
    {
      positions[i] = modelMatrix * inPositions[index[i]];
      outPositions[i] = mvpMatrix * inPositions[index[i]];

      outUVs[i] = inUVs[index[i]];

      outNormals[i] = normalMatrix * inNormals[index[i]];
      tangents[i] = normalMatrix * inTangents[index[i]];
      bitangents[i] = cross(outNormals[i],tangents[i]);

      outTbnMatrix[i] = glm::mat3(tangents[i], bitangents[i], outNormals[i]);
    }
  }

  virtual Color fragment(glm::vec3 bc)
  {
    glm::vec2 uv = outUVs[0] * bc[0] + outUVs[1] * bc[1] + outUVs[2] * bc[2];
    glm::vec3 position = positions[0] * bc[0] + positions[1] * bc[1] + positions[2] * bc[2];
    glm::mat3 interpolatedTBN = (outTbnMatrix[0] * bc[0]) + (outTbnMatrix[1] * bc[1]) + (outTbnMatrix[2] * bc[2]);
    
    glm::vec3 albedo = inMaterial->mAlbedoTexture->value(uv.x,uv.y).mColor;
    glm::vec3 metallicRoughness = inMaterial->mMetallicRoughness->value(uv.x,uv.y).mColor;
    glm::vec3 normal = interpolatedTBN * inMaterial->mNormalTexture->value(uv.x,uv.y).mColor;

    // glm::vec3 ao = inMaterial->mAOTexture->value(uv.x,uv.y).mColor;

    glm::vec3 viewDirection = normalize(inCamera->GetPosition() - position);

    glm::vec3 F0(0.04f);
    F0 = mix(F0, albedo, metallicRoughness.b);

    glm::vec3 L0(0.0f);
    for (size_t l = 0; l < inLights.size(); l++)
    {
      glm::vec3 lightDirection;
      switch (inLights[l]->mType)
      {
        case LightType::DIRECTIONAL:
        lightDirection = inLights[l]->mDirection;
        break;

        case LightType::POINT:
        lightDirection = glm::normalize(inLights[l]->GetPosition() - position);
        break;
      }
      glm::vec3 halfway = glm::normalize(viewDirection + lightDirection);

      float distance = glm::length(inLights[l]->GetPosition() - position);
      float attenuation = 1.0f / distance * distance;
      glm::vec3 radiance = inLights[l]->mDiffuseColor * attenuation;

      float NDF = DistributionGGX(normal, halfway, metallicRoughness.g);
      float G = GeometrySmith(normal, viewDirection, lightDirection, metallicRoughness.g);
      glm::vec3 F = fresnelSchlick(glm::max(glm::dot(halfway, viewDirection), 0.0f), F0);

      glm::vec3 kS = F;
      glm::vec3 kD = glm::vec3(1.0) - kS;
      kD *= 1.0 - metallicRoughness.b;

      glm::vec3 numerator = NDF * G * F;
      float denominator = 4.0 * glm::max(glm::dot(normal, viewDirection), 0.0f) * glm::max(glm::dot(normal, lightDirection), 0.0f) + 0.00001f;
      glm::vec3 specular = numerator / denominator;

      float NdotL = glm::max(glm::dot(normal, lightDirection), 0.0f);
      L0 += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    glm::vec3 ambient = glm::vec3(0.03f) * albedo;
    glm::vec3 color = ambient + L0;

    color = color / (color + glm::vec3(1.0f));
    color = pow(color, glm::vec3(1.0f/2.2f));

    return Color(color);
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

  glm::vec3 fresnelSchlick(float cosTheta, glm::vec3 F0)
  {
    return F0 + (glm::vec3(1.0) - F0) * pow(glm::clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
  }

  float DistributionGGX(glm::vec3 N, glm::vec3 H, float roughness)
  {
      float a2     = roughness * roughness;
      float NdotH  = glm::max(glm::dot(N, H), 0.0f);
      float NdotH2 = NdotH*NdotH;

      float num   = a2;
      float denom = (NdotH2 * (a2 - 1.0) + 1.0);
      denom = PI * denom * denom;

      return num / denom;
  }

  float GeometrySchlickGGX(float NdotV, float roughness)
  {
      float r = (roughness + 1.0);
      float k = (r*r) / 8.0;

      float num   = NdotV;
      float denom = NdotV * (1.0 - k) + k;

      return num / denom;
  }

  float GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float roughness)
  {
      float NdotV = glm::max(glm::dot(N, V), 0.0f);
      float NdotL = glm::max(glm::dot(N, L), 0.0f);
      float ggx2  = GeometrySchlickGGX(NdotV, roughness);
      float ggx1  = GeometrySchlickGGX(NdotL, roughness);

      return ggx1 * ggx2;
  }
};
