#pragma once

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <vector>
#include <string>
#include <object.h>
#include <camera.h>
#include <node.h>
#include <memory>

class Scene {
public:
  Scene(std::string scenePath);

  uint32_t mActiveCameraIndex;
  std::shared_ptr<Node> mRootNode;

  std::vector<Object> mObjects;
  std::vector<Camera> mCameras;
private:
  Scene() {}
  std::shared_ptr<Node> FindNode(std::shared_ptr<Node> node, std::string name);
  std::shared_ptr<Node> ProcessAssimpNode(aiNode* aiNode, std::shared_ptr<Node> parent);
};
