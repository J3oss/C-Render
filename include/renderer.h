#pragma once

#include <window.h>
#include <point.h>
#include <color.h>
#include <scene.h>

#include <vector>

class Renderer
{
public:
  static Renderer& GetInstance()
  {
    static Renderer instance;
    return instance;
  }

  void Update();
  void SetScene(uint32_t sceneIndex);

private:
  Renderer();
  Renderer(Renderer const&);
  void operator=(Renderer const&);

  Window _window;
  uint32_t _scene_index = 0;

  void DrawScene();

  void DrawObject(uint32_t _object_index);

  void DrawTriangle(Point p1, Point p2, Point p3, Color c);

  void DrawLine(Point p1, Point p2, Color c);
};
