#include <scene.h>

#include <iostream>
#include <var.h>

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <mesh.h>

#include <ext/glm/glm/glm.hpp>

glm::mat4 ConvertAssimpToGlmMat(aiMatrix4x4 aiMat)
{
  glm::mat4 mat;

  mat[0].x = aiMat[0][0];
  mat[1].x = aiMat[0][1];
  mat[2].x = aiMat[0][2];
  mat[3].x = aiMat[0][3];

  mat[0].y = aiMat[1][0];
  mat[1].y = aiMat[1][1];
  mat[2].y = aiMat[1][2];
  mat[3].y = aiMat[1][3];

  mat[0].z = aiMat[2][0];
  mat[1].z = aiMat[2][1];
  mat[2].z = aiMat[2][2];
  mat[3].z = aiMat[2][3];

  mat[0].w = aiMat[3][0];
  mat[1].w = aiMat[3][1];
  mat[2].w = aiMat[3][2];
  mat[3].w = aiMat[3][3];

  return mat;
}

Scene::Scene(std::string scenePath)
{
  Assimp::Importer importer;
  auto aiScene = importer.ReadFile(scenePath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(aiScene == nullptr || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  mRootNode = ProcessAssimpNode(aiScene->mRootNode ,nullptr);

  ProcessAssimpCameras(aiScene);

//   for(unsigned int meshIndex = 0; meshIndex < aiScene->mNumMeshes; meshIndex++)
//   {
//     Object object;
//     auto aiMesh = aiScene->mMeshes[meshIndex];
//
//     for(unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; faceIndex++)
//     {
//       uint32_t I1 = aiMesh->mFaces[faceIndex].mIndices[0];
//       uint32_t I2 = aiMesh->mFaces[faceIndex].mIndices[1];
//       uint32_t I3 = aiMesh->mFaces[faceIndex].mIndices[2];
//
//       object.mesh.mIndices.push_back(I1);
//       object.mesh.mIndices.push_back(I2);
//       object.mesh.mIndices.push_back(I3);
//     }
//
//     glm::vec4 pos;
//     for (size_t vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
//     {
//       pos.x = aiMesh->mVertices[vertexIndex].x;
//       pos.y = aiMesh->mVertices[vertexIndex].y;
//       pos.z = aiMesh->mVertices[vertexIndex].z;
//       pos.w = 1;
//
//       object.mesh.mPositions.push_back(pos);
//     }
//
//     mObjects.push_back(object);
//   }
}

void Scene::ProcessAssimpCameras(const aiScene* aiScene)
{
  if (!aiScene->HasCameras())
  {
    //new camera
    auto newCamera = new Camera();
    newCamera->SetName( "DefaultCamera" );
    newCamera->SetParent( mRootNode );

    glm::mat4 transform = glm::mat4(1.0);
    transform[2].z *= -1.0f;
    newCamera->SetLocalTransform( transform );

    //replacing pointers
    std::shared_ptr<Node> cameraNode(newCamera);
    mRootNode->AddChild(cameraNode);
    mCameras.push_back( std::static_pointer_cast<Camera>(cameraNode));
  }

  for (size_t cameraIndex = 0; cameraIndex < aiScene->mNumCameras; cameraIndex++)
  {
    //assimp camera
    auto aiCamera = aiScene->mCameras[cameraIndex];
    float aiFov = aiCamera->mHorizontalFOV;
    float aiAspect = aiCamera->mAspect;
    float aiNear = aiCamera->mClipPlaneNear;
    float aiFar = aiCamera->mClipPlaneFar;

    //scene graph camera node
    auto cameraNode = FindNode(mRootNode, aiCamera->mName.C_Str());

    //new camera to replace the scene graph camera node
    auto newCamera = new Camera(aiFov, 1.0f, aiNear, aiFar);
    newCamera->SetName( aiCamera->mName.C_Str() );
    newCamera->SetParent( cameraNode->GetParent() );
    newCamera->SetLocalTransform( cameraNode->GetLocalTransform() );
    newCamera->mChildren = std::move(cameraNode->mChildren);

    //replacing pointers
    cameraNode.reset(newCamera);
    mCameras.push_back( std::static_pointer_cast<Camera>(cameraNode));
  }
}

std::shared_ptr<Node> Scene::ProcessAssimpNode(aiNode* aiNode, std::shared_ptr<Node> parent)
{
  auto node = new Node();
  node->SetParent(parent);
  node->SetName(aiNode->mName.C_Str());
  node->SetLocalTransform(ConvertAssimpToGlmMat(aiNode->mTransformation));

  std::shared_ptr<Node> pNode(node);
  for (size_t childIndex = 0; childIndex < aiNode->mNumChildren; childIndex++)
  {
    auto c = ProcessAssimpNode(aiNode->mChildren[childIndex], pNode);
    std::shared_ptr<Node> pChild(c);
    node->AddChild(pChild);
  }

  return pNode;
}

std::shared_ptr<Node> Scene::FindNode(std::shared_ptr<Node> node, std::string name)
{
  if (node->GetName().compare(name) == 0)
    return node;

  std::shared_ptr<Node> foundNode = nullptr;
  for (size_t childIndex = 0; childIndex < node->mChildren.size(); childIndex++)
  {
    foundNode = FindNode(node->mChildren[childIndex], name);
    if (foundNode != nullptr)
      break;
  }

  return foundNode;
}
