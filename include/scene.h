#pragma once

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <vector>
#include <string>
#include <object.h>

class Scene {
public:
  Scene(std::string scenePath);

  std::vector<Object> _objects;
private:
  Scene() {}
};
