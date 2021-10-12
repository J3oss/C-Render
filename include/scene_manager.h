#include <scene.h>
#include <memory>
#include <vector>

#include <var.h>

class SceneManager
{
public:
  void AddScene(std::shared_ptr<Scene> scene);

  std::shared_ptr<Scene> GetActiveScene();
  std::shared_ptr<Scene> NextActiveScene();

  std::shared_ptr<Node> GetFocusedObject(FocusType type);
  std::shared_ptr<Node> NextFocusedObject(FocusType type);
private:
  FocusType mFocusObjectType = FocusType::CAMERAS;
  uint32_t mFocusObjectIndex = 0;

  uint32_t mActiveSceneIndex = 0;

  std::vector< std::shared_ptr<Scene> > mScenes;
};
