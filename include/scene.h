#pragma once

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <vector>
#include <string>
#include <camera.h>
#include <node.h>
#include <memory>
#include <mesh.h>

class Scene {
public:
  Scene(std::string scenePath);

  std::shared_ptr<Node> mRootNode;

  uint32_t mActiveCameraIndex = 0;
  std::vector< std::shared_ptr<Camera> > mCameras;

  std::vector<Mesh> mMeshes;

private:
  std::shared_ptr<Node> FindNode(std::shared_ptr<Node> node, std::string name);
  std::shared_ptr<Node> ProcessAssimpNode(aiNode* aiNode, std::shared_ptr<Node> parent);
  void ProcessAssimpCameras(const aiScene* aiScene);
};
