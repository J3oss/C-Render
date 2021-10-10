#pragma once

#include <textures/texture.h>
#include <string>
#include <memory>

class Material {
public:
  std::string mName;
  std::shared_ptr<Texture> mAlbedoTexture;
  std::shared_ptr<Texture> mNormalTexture;

  Material()
  {

  }
};
