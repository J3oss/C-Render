#pragma once

#include <window.h>
#include <color.h>
#include <scene.h>

#include <vector>
#include <memory>
#include <ext/glm/glm/glm.hpp>

enum class CullingMode { NONE, FRONT, BACK};
enum class DepthTest { NONE, GREATER, LESS};

class Renderer
{
public:
  Renderer();

  void Update();
  void DrawScene(std::shared_ptr<Scene> scene);
  void Clear();

  void SetViewPort(int x, int y);
  void SetDepthTest(DepthTest test);
  void SetCullMode(CullingMode mode);
private:
  std::vector<float> zBuffer;
  Window _window;
  glm::mat4 mViewPort;

  DepthTest mDepthTest = DepthTest::NONE;
  CullingMode mCullMode = CullingMode::NONE;

  void DrawMesh(std::shared_ptr<Scene> scene, uint32_t meshIndex);

  void DrawTriangle(glm::ivec3 p1, glm::ivec3 p2, glm::ivec3 p3);

  void DrawTriangleSolid(glm::ivec3 x, glm::ivec3 y, glm::vec3 z, glm::vec2 uv1,glm::vec2 uv2,glm::vec2 uv3, float intensity, Texture* texture);

  void DrawLine(glm::ivec2 p1, glm::ivec2 p2, Color c);
};
