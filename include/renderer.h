#pragma once

#include <window.h>
#include <point.h>
#include <color.h>
#include <scene.h>

#include <vector>

class Renderer
{
public:
  Renderer();

  void Update();
  void DrawScene(std::shared_ptr<Scene> scene);

private:
  Window _window;

  void DrawObject(std::shared_ptr<Scene> scene, uint32_t _object_index);

  void DrawTriangle(Point p1, Point p2, Point p3, Color c);

  void DrawLine(Point p1, Point p2, Color c);
};
