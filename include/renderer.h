#pragma once

#include <window.h>
#include <point.h>
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
  Window _window;
  glm::mat4 mViewPort;

  void DrawObject(std::shared_ptr<Scene> scene, uint32_t _object_index);

  void DrawTriangle(Point p1, Point p2, Point p3, Color c);

  void DrawLine(Point p1, Point p2, Color c);
};
