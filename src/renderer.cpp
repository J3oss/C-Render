#include <renderer.h>

#define abs(x) (x > 0 ? x:-1 * x)
union FPint
{
  int32_t i;

  struct
  {
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int16_t hi;
    int16_t lo;
    #else
    int16_t lo;
    int16_t hi;
    #endif
  };
};

void Renderer::DrawLine(Point &p1, Point &p2, Color c)
{
  Point d;
  d = p2 - p1;

  if (abs(d.x) > abs(d.y))
  {
    if (p1.x > p2.x) {
      Point::Swap(p1, p2);
    }

    FPint y;
    y.i = p1.y<<16;

    int32_t m = ((int32_t)(p2.y - p1.y)<<16)/(p2.x - p1.x);

    Point p;
    for (size_t x = p1.x; x <= p2.x; x++, y.i+=m)
    {
      p.x = x;
      p.y = y.hi;
      _window.SetPixel(p, c);
    }
  }
  else
  {
    if (p1.y > p2.y) {
      Point::Swap(p1, p2);
    }

    FPint x;
    x.i = p1.x<<16;

    int32_t m = ((int32_t)(p2.x - p1.x)<<16)/(p2.y - p1.y);

    Point p;
    for (size_t y = p1.y; y <= p2.y; y++, x.i+=m)
    {
      p.x = x.hi;
      p.y = y;
      _window.SetPixel(p, c);
    }
  }
}

void Renderer::Update()
{
  _window.Update();
}
