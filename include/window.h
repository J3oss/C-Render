#pragma once

#include <ext/SDL/include/SDL.h>

#include <string>

#include <point.h>
#include <color.h>

class Window
{
public:
  Window() {};
  Window(const std::string window_name, uint32_t width,uint32_t height);

  void Update();

  void SetPixel(Point p, Color c);

  void Clear();

private:
  uint32_t* _pixels;

  SDL_Window* _window;
  SDL_Surface* _surface;
  SDL_Surface* _temp_surface;
};
