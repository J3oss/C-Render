#include <scene.h>

#include <iostream>
#include <var.h>

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <mesh.h>

Scene::Scene(std::string scenePath)
{
  Assimp::Importer importer;
  auto aiScene = importer.ReadFile(scenePath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(aiScene == nullptr || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  Camera c;
  mCameras.push_back(c);
  mActiveCameraIndex = 0;

  for (size_t cameraIndex = 0; cameraIndex < aiScene->mNumCameras; cameraIndex++)
  {
    auto aiCamera = aiScene->mCameras[cameraIndex];

    //TODO process cameras from scene
    Camera c;
    mCameras.push_back(c);
  }

  for(unsigned int meshIndex = 0; meshIndex < aiScene->mNumMeshes; meshIndex++)
  {
    Object object;
    auto aiMesh = aiScene->mMeshes[meshIndex];

    for(unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; faceIndex++)
    {
      uint32_t I1 = aiMesh->mFaces[faceIndex].mIndices[0];
      uint32_t I2 = aiMesh->mFaces[faceIndex].mIndices[1];
      uint32_t I3 = aiMesh->mFaces[faceIndex].mIndices[2];

      object.mesh.mIndices.push_back(I1);
      object.mesh.mIndices.push_back(I2);
      object.mesh.mIndices.push_back(I3);
    }

    glm::vec4 pos;
    for (size_t vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
    {
      pos.x = aiMesh->mVertices[vertexIndex].x;
      pos.y = aiMesh->mVertices[vertexIndex].y;
      pos.z = aiMesh->mVertices[vertexIndex].z;
      pos.w = 1;

      object.mesh.mPositions.push_back(pos);
    }

    mObjects.push_back(object);
  }
}
