#include <scene_manager.h>

typedef uint32_t SceneHandle;

void SceneManager::AddScene(std::shared_ptr<Scene> scene)
{
  mScenes.push_back(scene);
}

std::shared_ptr<Scene> SceneManager::GetActiveScene()
{
  mActiveSceneIndex %= mScenes.size();
  return mScenes[mActiveSceneIndex];
}
std::shared_ptr<Scene> SceneManager::NextActiveScene()
{
  mFocusObjectIndex = 0;
  mActiveSceneIndex++;
  printf("Focus on Scene number: %ld\n", mActiveSceneIndex%mScenes.size());
  return GetActiveScene();
}


std::shared_ptr<Node> SceneManager::GetFocusedObject(FocusType type)
{
  if (mFocusObjectType != type)
  {
    mFocusObjectType = type;
    mFocusObjectIndex = 0;
  }

  std::shared_ptr<Node> object;
  switch (type)
  {
    case FocusType::CAMERAS:
    mFocusObjectIndex %= GetActiveScene()->mCameras.size();
    printf("Focus on camera number: %d\n", mFocusObjectIndex);
    object = GetActiveScene()->mCameras[mFocusObjectIndex];
    break;
    case FocusType::MESHES:
    mFocusObjectIndex %= GetActiveScene()->mMeshes.size();
    printf("Focus on mesh number: %d\n", mFocusObjectIndex);
    object = GetActiveScene()->mMeshes[mFocusObjectIndex];
    break;
    // case FocusType::LIGHTS:
    // mFocusObjectIndex %= GetActiveScene()->mLights.size();
    // object = GetActiveScene()->mLights[mFocusObjectIndex];
    // printf("Focus on light number: %d\n", mFocusObjectIndex);
    // break;
  }

  return object;
}
std::shared_ptr<Node> SceneManager::NextFocusedObject(FocusType type)
{
  mFocusObjectIndex++;

  return GetFocusedObject(type);
}
