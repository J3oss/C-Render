#include <scene_manager.h>

typedef uint32_t SceneHandle;

void SceneManager::AddScene(std::shared_ptr<Scene> scene)
{
  mScenes.push_back(scene);
}

void SceneManager::NextActiveScene()
{
  mActiveSceneIndex++;

  mActiveSceneIndex %= mScenes.size();
}

void SceneManager::SetActiveScene(uint32_t index)
{
  mActiveSceneIndex = index;
}

std::shared_ptr<Scene> SceneManager::GetActiveScene()
{
  return mScenes[mActiveSceneIndex];
}
