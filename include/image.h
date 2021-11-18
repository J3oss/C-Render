#pragma once

#include <string>
#include <vector>
#include <color.h>

enum class ImageUsage {DEFAULT, SRGB, NORMAL};

class Image {
public:
  int mWidth, mHeight, mChannels;

  std::vector<Color> mPixels;

  Image() {}
  Image(const char* path, ImageUsage usage);
};
