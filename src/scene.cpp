#include <scene.h>

#include <iostream>
#include <var.h>

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <mesh.h>

std::vector<Scene> Scene::_scenes;

Scene::Scene(std::string scenePath)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(scenePath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  Scene s;

  for(unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
  {
    Object object;
    aiMesh *aimesh = scene->mMeshes[meshIndex];

    for(unsigned int faceIndex = 0; faceIndex < aimesh->mNumFaces; faceIndex++)
    {
      uint32_t I1 = aimesh->mFaces[faceIndex].mIndices[0];
      uint32_t I2 = aimesh->mFaces[faceIndex].mIndices[1];
      uint32_t I3 = aimesh->mFaces[faceIndex].mIndices[2];

      object.mesh._indices.push_back(I1);
      object.mesh._indices.push_back(I2);
      object.mesh._indices.push_back(I3);
    }

    for (size_t vertexIndex = 0; vertexIndex < aimesh->mNumVertices; vertexIndex++)
    {
      Point p( (aimesh->mVertices[vertexIndex].x + 1.0) * WIDTH/2,
               (aimesh->mVertices[vertexIndex].y + 1.0) * HEIGHT/2,
              aimesh->mVertices[vertexIndex].z);

      object.mesh._vertices.push_back(p);
    }
    s._objects.push_back(object);
  }

  _scenes.push_back(s);
}
