#include <image.h>


#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb/stb_image.h>
#include <iostream>

Image::Image(const char* path, ImageType type)
{
  if ( type == ImageType::NORMAL )
  {
    std::string fpath = std::string("res/") + std::string(path);
    float* pixels = stbi_loadf(fpath.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb);

    if (pixels == nullptr) {
      printf("texture not found\n");
      return;
    }

    Color c;
    for (size_t pixelIndex = 0; pixelIndex < mWidth*mHeight*mChannels ; pixelIndex+=mChannels)
    {
      c.r = pixels[pixelIndex];
      c.g = pixels[pixelIndex+1];
      c.b = pixels[pixelIndex+2];

      mPixels.push_back(c);
    }

    free(pixels);
  }
  else
  {
    std::string fpath = std::string("res/") + std::string(path);
    stbi_uc* pixels = stbi_load(fpath.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb);

    if (pixels == nullptr) {
      printf("texture not found\n");
      return;
    }

    Color c;
    for (size_t pixelIndex = 0; pixelIndex < mWidth*mHeight*mChannels; pixelIndex+=mChannels
    )
    {
      c.r = pixels[pixelIndex];
      c.g = pixels[pixelIndex+1];
      c.b = pixels[pixelIndex+2];
      // c.a = pixels[pixelIndex+3];

      mPixels.push_back(c);
    }

    free(pixels);
  }
}

// Image::Image(const char* path, ImageType type)
// {
//   std::string fpath = std::string("res/") + std::string(path);
//   switch (type)
//   {
//     case ImageType::DIFFUSE:
//     {
//       printf("here d\n");
//       auto pixels = stbi_load(fpath.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
//       break;
//
//       if (pixels == nullptr) {
//         printf("texture not found\n");
//         return;
//       }
//
//       Color c;
//       for (size_t pixelIndex = 0; pixelIndex < mWidth*mHeight*mChannels ; pixelIndex+=mChannels)
//       {
//         c.r = pixels[pixelIndex];
//         c.g = pixels[pixelIndex+1];
//         c.b = pixels[pixelIndex+2];
//         c.a = pixels[pixelIndex+3];
//
//         mPixels.push_back(c);
//       }
//       break;
//     }
//
//     case ImageType::NORMAL:
//     {
//             printf("here n\n");
//       auto pixels = stbi_loadf(fpath.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb);
//
//       if (pixels == nullptr) {
//         printf("texture not found\n");
//         return;
//       }
//
//       Color c;
//       for (size_t pixelIndex = 0; pixelIndex < mWidth*mHeight*3 ; pixelIndex+=3)
//       {
//         c.r = pixels[pixelIndex];
//         c.g = pixels[pixelIndex+1];
//         c.b = pixels[pixelIndex+2];
//
//         mPixels.push_back(c);
//       }
//       break;
//     }
//   }
// }
