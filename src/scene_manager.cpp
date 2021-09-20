#include <scene_manager.h>

typedef uint32_t SceneHandle;

void SceneManager::AddScene(std::shared_ptr<Scene> scene)
{
  mScenes.push_back(scene);
}

std::shared_ptr<Scene> SceneManager::GetScene(uint32_t sceneIndex)
{
  return mScenes[sceneIndex];
}
