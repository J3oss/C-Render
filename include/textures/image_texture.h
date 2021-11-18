#pragma once

#include <textures/texture.h>
#include <image.h>

class ImageTexture : public Texture
{
public:
  ImageTexture(const char* path, ImageUsage usage) { mImage = Image(path, usage); }

  virtual Color value(float u, float v) const override
  {
    u *= mImage.mWidth;
    v *= mImage.mHeight;

    uint32_t FinalU = (uint32_t)u % mImage.mWidth;
    uint32_t FinalV = (uint32_t)v % mImage.mHeight;

    return mImage.mPixels[ FinalV * mImage.mWidth + FinalU];
  }

private:
  Image mImage;
};
