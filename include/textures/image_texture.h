#pragma once

#include <textures/texture.h>
#include <string>
#include <image.h>
#include <iostream>

class ImageTexture : public Texture
{
public:
  ImageTexture(const char* path, ImageType type) { mImage = Image(path, type); }

  virtual Color value(float u, float v) const override
  {
    u *= (float)mImage.mWidth;
    v *= (float)mImage.mHeight;

    uint32_t FinalU = (uint32_t)u % mImage.mWidth;
    uint32_t FinalV = (uint32_t)v % mImage.mHeight;

    // printf("%d %d\n", FinalU, FinalV);

    return mImage.mPixels[ ((mImage.mHeight - FinalV) * mImage.mWidth) + FinalU];
  }

private:
  Image mImage;
};
