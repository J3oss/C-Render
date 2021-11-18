#include <image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb/stb_image.h>
#include <iostream>

Image::Image(const char* path, ImageUsage usage)
{
  std::string fpath = std::string("res/") + std::string(path);
  stbi_uc* pixels = stbi_load(fpath.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb);

  if (pixels == nullptr) {
    printf("texture not found\n");
    return;
  }

  Color c;
  for (size_t pixelIndex = 0; pixelIndex < mWidth*mHeight*mChannels; pixelIndex+=mChannels)
  {
    c.mColor.x = static_cast<float>(pixels[pixelIndex])/255.0f;
    c.mColor.y = static_cast<float>(pixels[pixelIndex+1])/255.0f;
    c.mColor.z = static_cast<float>(pixels[pixelIndex+2])/255.0f;

    switch (usage)
    {
      case ImageUsage::SRGB:
      c.mColor.x = pow(c.mColor.x, 2.2f);
      c.mColor.y = pow(c.mColor.y, 2.2f);
      c.mColor.z = pow(c.mColor.z, 2.2f);
      break;
      case ImageUsage::NORMAL:
      c.mColor = glm::normalize(c.mColor * glm::vec3(2.0f) - glm::vec3(1.0f));
      break;
      case ImageUsage::DEFAULT:
      break;
    }

    mPixels.push_back(c);
  }

  free(pixels);
}
