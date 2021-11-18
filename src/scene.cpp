#include <scene.h>

#include <iostream>
#include <var.h>

#include <ext/assimp/include/assimp/Importer.hpp>
#include <ext/assimp/include/assimp/scene.h>
#include <ext/assimp/include/assimp/postprocess.h>

#include <mesh.h>

#include <ext/glm/glm/glm.hpp>

#include <textures/texture.h>
#include <textures/solid_texture.h>
#include <textures/image_texture.h>

#include <glm/gtx/matrix_decompose.hpp>

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
  printf("Importing Scene: %s\n", scenePath.c_str());

  Assimp::Importer importer;
  auto aiScene = importer.ReadFile(scenePath, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(aiScene == nullptr || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  ProcessAssimpMaterials(aiScene);
  mRootNode = ProcessAssimpNodes(aiScene, aiScene->mRootNode, mRootNode);

  ProcessAssimpLights(aiScene);
  ProcessAssimpCameras(aiScene);

  printf("\n");
}

std::shared_ptr<Node> Scene::ProcessAssimpNodes(const aiScene* aiScene, aiNode* aiNode, std::weak_ptr<Node> parent)
{
  auto node = std::make_shared<Node>();

  node->SetParent(parent);
  node->SetName(aiNode->mName.C_Str());
  node->SetLocalTransform(ConvertAssimpToGlmMat(aiNode->mTransformation));

  for (size_t meshIndex = 0; meshIndex < aiNode->mNumMeshes; meshIndex++)
    ProcessAssimpMesh(aiScene->mMeshes[aiNode->mMeshes[meshIndex]], node->GetParent());

  for (size_t childIndex = 0; childIndex < aiNode->mNumChildren; childIndex++)
    node->AddChild(ProcessAssimpNodes(aiScene, aiNode->mChildren[childIndex], node));

  return node;
}

void Scene::ProcessAssimpMaterials(const aiScene* aiScene)
{
  if (!aiScene->HasMaterials())
    return;

  for (size_t materialIndex = 0; materialIndex < aiScene->mNumMaterials; materialIndex++)
  {
    auto newMaterial = std::make_shared<Material>();

    printf("  Material Number: %ld\n", materialIndex);
    auto aiMaterial = aiScene->mMaterials[materialIndex];

    aiString matName;
    aiMaterial->Get(AI_MATKEY_NAME, matName);
    printf("    Material Name: %s\n", matName.C_Str());
    newMaterial->mName = std::string(matName.C_Str());

    aiString texPath;
    if ( aiMaterial->Get(AI_MATKEY_TEXTURE(  aiTextureType_DIFFUSE , 0), texPath) == AI_SUCCESS )
    {
      aiMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, materialIndex), texPath);
      auto newTexture = std::make_shared<ImageTexture>(texPath.C_Str(), ImageUsage::SRGB);
      newMaterial->mAlbedoTexture = newTexture;
    }
    else
    {
      aiColor3D baseColor;
      aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor);

      auto newTexture = std::make_shared<SolidTexture>(255*baseColor.r, 255*baseColor.g, 255*baseColor.b);
      newMaterial->mAlbedoTexture = newTexture;
    }

    if ( aiMaterial->Get(AI_MATKEY_TEXTURE(  aiTextureType_NORMALS , 0), texPath) == AI_SUCCESS )
    {
      aiMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, materialIndex), texPath);
      auto newTexture = std::make_shared<ImageTexture>(texPath.C_Str(), ImageUsage::NORMAL);
      newMaterial->mNormalTexture = newTexture;
    }

    if ( aiMaterial->Get(AI_MATKEY_TEXTURE(  aiTextureType_UNKNOWN , 0), texPath) == AI_SUCCESS )
    {
      aiMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_UNKNOWN, materialIndex), texPath);
      auto newTexture = std::make_shared<ImageTexture>(texPath.C_Str(), ImageUsage::DEFAULT);
      newMaterial->mMetallicRoughness = newTexture;
    }


    mMaterials.push_back( newMaterial );
  }
}

void Scene::ProcessAssimpCameras(const aiScene* aiScene)
{
  if (!aiScene->HasCameras())
  {
    //new camera
    auto newCamera = std::make_shared<Camera>();
    newCamera->SetName( std::string("DefaultCamera") );
    newCamera->SetParent( mRootNode );

    glm::mat4 transform = glm::mat4(1.0);
    transform[2].z *= -1.0f;
    newCamera->SetLocalTransform( transform );

    //adding new node
    mRootNode->AddChild(newCamera);
    mCameras.push_back(newCamera);
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
    auto cameraNode = FindNode(mRootNode, std::string(aiCamera->mName.C_Str()));

    //new camera to replace the scene graph camera node
    auto newCamera = std::make_shared<Camera>(aiFov, 1.0f, aiNear, aiFar);
    newCamera->SetName( std::string(std::string(aiCamera->mName.C_Str())) );
    newCamera->SetParent( cameraNode->GetParent() );
    newCamera->SetLocalTransform( cameraNode->GetLocalTransform() );
    newCamera->mChildren = std::move(cameraNode->mChildren);

    //replacing pointers
    cameraNode = newCamera;
    mCameras.push_back(newCamera);
  }
}

void Scene::ProcessAssimpLights(const aiScene* aiScene)
{
  for (size_t lightIndex = 0; lightIndex < aiScene->mNumLights; lightIndex++)
  {
    //assimp light
    auto aiLight = aiScene->mLights[lightIndex];

    //scene graph light node
    auto lightNode = FindNode(mRootNode, std::string(aiLight->mName.C_Str()));

    //new light to replace the scene graph node
    auto newLight = std::make_shared<Light>();
    newLight->SetName( std::string(std::string(aiLight->mName.C_Str())) );
    newLight->SetParent( lightNode->GetParent() );
    newLight->SetLocalTransform( lightNode->GetLocalTransform() );
    newLight->mChildren = std::move(lightNode->mChildren);

    newLight->mDirection = glm::vec4(lightNode->GetGlobalTransform()[2]);
    newLight->mConstant = aiLight->mAttenuationConstant;
    newLight->mLinear = aiLight->mAttenuationLinear;
    newLight->mQuadratic = aiLight->mAttenuationQuadratic;
    newLight->mAmbientColor = glm::vec3(aiLight->mColorAmbient.r, aiLight->mColorAmbient.g, aiLight->mColorAmbient.b);
    newLight->mDiffuseColor = glm::vec3(aiLight->mColorDiffuse.r, aiLight->mColorDiffuse.g, aiLight->mColorDiffuse.b);
    newLight->mSpecularColor = glm::vec3(aiLight->mColorSpecular.r, aiLight->mColorSpecular.g, aiLight->mColorSpecular.b);

    switch (aiLight->mType)
    {
      case aiLightSource_POINT:
      newLight->mType = LightType::POINT;
      break;
      case aiLightSource_DIRECTIONAL:
      newLight->mType = LightType::DIRECTIONAL;
      break;
    }

    //replacing pointers
    lightNode = newLight;
    mLights.push_back(newLight);
  }
}

void Scene::ProcessAssimpMesh(aiMesh* aiMesh, std::shared_ptr<Node> parentNode)
{
  auto newmesh = std::make_shared<Mesh>();

  for(unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; faceIndex++)
  {
    uint32_t I1 = aiMesh->mFaces[faceIndex].mIndices[0];
    uint32_t I2 = aiMesh->mFaces[faceIndex].mIndices[1];
    uint32_t I3 = aiMesh->mFaces[faceIndex].mIndices[2];

    newmesh->mIndices.push_back(I1);
    newmesh->mIndices.push_back(I2);
    newmesh->mIndices.push_back(I3);
  }

  glm::vec4 pos;
  for (size_t vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
  {
    pos.x = aiMesh->mVertices[vertexIndex].x;
    pos.y = aiMesh->mVertices[vertexIndex].y;
    pos.z = aiMesh->mVertices[vertexIndex].z;
    pos.w = 1;

    newmesh->mPositions.push_back(pos);
  }

  glm::vec4 norm;
  for (size_t normalIndex = 0; normalIndex < aiMesh->mNumVertices; normalIndex++)
  {
    norm.x = aiMesh->mNormals[normalIndex].x;
    norm.y = aiMesh->mNormals[normalIndex].y;
    norm.z = aiMesh->mNormals[normalIndex].z;
    norm.w = 0;

    newmesh->mNormals.push_back(norm);
  }

  glm::vec4 tang;
  for (size_t tangentIndex = 0; tangentIndex < aiMesh->mNumVertices; tangentIndex++)
  {
    tang.x = aiMesh->mNormals[tangentIndex].x;
    tang.y = aiMesh->mNormals[tangentIndex].y;
    tang.z = aiMesh->mNormals[tangentIndex].z;
    tang.w = 0;

    newmesh->mTangets.push_back(tang);
  }

  if (aiMesh->GetNumUVChannels() > 0)
  {
    glm::vec2 uv;
    for (size_t uvIndex = 0; uvIndex < aiMesh->mNumVertices; uvIndex++)
    {
      uv.x = aiMesh->mTextureCoords[0][uvIndex].x;
      uv.y = aiMesh->mTextureCoords[0][uvIndex].y;

      newmesh->mUVs.push_back(uv);
    }
  }

  newmesh->mMaterial = mMaterials[aiMesh->mMaterialIndex];

  newmesh->SetName( std::string("mesh") );
  newmesh->SetParent( parentNode );
  newmesh->SetLocalTransform( glm::mat4(1.0) );

  //adding new node
  parentNode->AddChild(newmesh);
  mMeshes.push_back(newmesh);
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
