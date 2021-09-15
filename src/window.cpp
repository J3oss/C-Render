#include <window.h>

Window::Window(const std::string window_name, uint32_t width,uint32_t height)
{
  SDL_Init(SDL_INIT_VIDEO);

  _window = SDL_CreateWindow(window_name.c_str(), 0, 0, width, height, SDL_WINDOW_SHOWN);
  _surface = SDL_GetWindowSurface(_window);

  uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  _temp_surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
  _pixels = (uint32_t *) _temp_surface->pixels;
}

void Window::Update()
{
  SDL_BlitSurface(_temp_surface, NULL, _surface, NULL);
  SDL_UpdateWindowSurface(_window);
}

void Window::SetPixel(Point p, Color c)
{
  //image is flipped horrizontally
  uint32_t pxCoordinate = p.x + (_temp_surface->h -1 - p.y) * _temp_surface->w;
  if (pxCoordinate > _temp_surface->h * _temp_surface->w) {
    printf("erro pxCoordinate out of range\n");
    return;
  }
  _pixels[pxCoordinate] = SDL_MapRGB(_temp_surface->format, c.r, c.g, c.b);
}
