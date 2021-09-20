#include <scene.h>
#include <memory>
#include <vector>

class SceneManager {
public:
  void AddScene(std::shared_ptr<Scene> scene);
  std::shared_ptr<Scene> GetScene(uint32_t scene);

private:
  std::vector< std::shared_ptr<Scene> > mScenes;
};
