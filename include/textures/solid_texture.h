#pragma once

#include <textures/texture.h>

class SolidTexture : public Texture
{
public:
  SolidTexture() {}
  SolidTexture(Color c) { mColor = c; }
  SolidTexture(float r, float g, float b) { mColor = Color(r,g,b); }

  virtual Color value(float u, float v) const override { return mColor; }

private:
  Color mColor;
};
