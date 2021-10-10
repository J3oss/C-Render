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
#include <material.h>

class Scene {
public:
  Scene(std::string scenePath);

  std::shared_ptr<Node> mRootNode;

  uint32_t mActiveCameraIndex = 0;
  std::vector< std::shared_ptr<Camera> > mCameras;

  std::vector< std::shared_ptr<Mesh> > mMeshes;

  std::vector< std::shared_ptr<Material> > mMaterials;

private:
  std::shared_ptr<Node> FindNode(std::shared_ptr<Node> node, std::string name);
  std::shared_ptr<Node> ProcessAssimpNodes(const aiScene* aiScene, aiNode* aiNode, std::weak_ptr<Node> parent);

  void ProcessAssimpCameras(const aiScene* aiScene);
  void ProcessAssimpMaterials(const aiScene* aiScene);
  void ProcessAssimpMesh(aiMesh* aiMesh, std::shared_ptr<Node> parentNode);
};
