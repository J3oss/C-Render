#pragma once

#include <window.h>
#include <point.h>
#include <color.h>
#include <scene.h>

#include <vector>

class Renderer
{
public:
  Renderer(uint32_t width,uint32_t height)
  {
    _window = Window("C-Render", width, height);
  }

  void Update();

  void SetScene(uint32_t sceneIndex);

private:
  Window _window;

  uint32_t _scene_index = 0;
  std::vector<Scene> _scenes;

  void DrawScene();

  void DrawObject(uint32_t _object_index);

  void DrawTriangle(Point p1, Point p2, Point p3, Color c);

  void DrawLine(Point p1, Point p2, Color c);
};
