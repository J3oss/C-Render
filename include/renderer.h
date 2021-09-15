#pragma once

#include <window.h>
#include <point.h>
#include <color.h>

class Renderer
{
public:
  Renderer(uint32_t width,uint32_t height)
  {
    _window = Window("C-Render", width, height);
  }

  void DrawLine(Point &p1, Point &p2, Color c);

  void Update();

private:
  Window _window;
};
