#pragma once

#include <window.h>
#include <color.h>
#include <scene.h>

#include <vector>
#include <memory>
#include <ext/glm/glm/glm.hpp>

class Renderer
{
public:
  Renderer();

  void Update();
  void DrawScene(std::shared_ptr<Scene> scene);
  void Clear();

  void SetViewPort(int x, int y);
private:
  float* zBuffer;
  Window _window;
  glm::mat4 mViewPort;

  void DrawMesh(std::shared_ptr<Scene> scene, uint32_t meshIndex);

  void DrawTriangle(glm::ivec3 p1, glm::ivec3 p2, glm::ivec3 p3);

  void DrawTriangleSolid(glm::ivec3 x, glm::ivec3 y, glm::vec3 z, float intensity);

  void DrawLine(glm::ivec2 p1, glm::ivec2 p2, Color c);
};
