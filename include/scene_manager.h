#include <scene.h>
#include <memory>
#include <vector>

class SceneManager {
public:
  void AddScene(std::shared_ptr<Scene> scene);

  void NextActiveScene();
  void SetActiveScene(uint32_t index);
  std::shared_ptr<Scene> GetActiveScene();

private:
  uint32_t mActiveSceneIndex = 0;
  std::vector< std::shared_ptr<Scene> > mScenes;
};
