#pragma once

#include <textures/texture.h>

class SolidTexture : public Texture
{
public:
  SolidTexture() {}
  SolidTexture(Color c) { mColor = c; }
  SolidTexture(char r, char g, char b) { mColor = Color(r,g,b, 255); }

  virtual Color value(float u, float v) const override { return mColor; }

private:
  Color mColor;
};
