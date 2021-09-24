#pragma once

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <vector>
#include <string>
#include <object.h>
#include <camera.h>

class Scene {
public:
  Scene(std::string scenePath);

  uint32_t mActiveCameraIndex;

  std::vector<Object> mObjects;
  std::vector<Camera> mCameras;
private:
  Scene() {}
};
