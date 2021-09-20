#include <renderer.h>

#include <var.h>

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

Renderer::Renderer()
{
  _window = Window("C-Render", WIDTH+1, HEIGHT+1);
}

void Renderer::Update()
{
  _window.Update();
}

void Renderer::Clear()
{
  _window.Clear();
}

void Renderer::DrawScene(std::shared_ptr<Scene> scene)
{
  for (size_t objIndex = 0; objIndex < scene->_objects.size(); objIndex++) {
    DrawObject(scene, objIndex);
  }
}

void Renderer::DrawObject(std::shared_ptr<Scene> scene, uint32_t _object_index)
{
  //remove this later
  Color white(255, 255, 255);
  Object o = scene->_objects[_object_index];

  //draw mesh
  for (size_t index = 0; index < o.mesh._indices.size(); index+=3) {
    uint32_t p1Index = o.mesh._indices[index];
    uint32_t p2Index = o.mesh._indices[index+1];
    uint32_t p3Index = o.mesh._indices[index+2];

    DrawTriangle(o.mesh._vertices[p1Index],
                 o.mesh._vertices[p2Index],
                 o.mesh._vertices[p3Index],
                 white);
  }
}

void Renderer::DrawTriangle(Point p1, Point p2, Point p3, Color c)
{
  DrawLine(p1, p2, c);
  DrawLine(p2, p3, c);
  DrawLine(p3, p1, c);
}

void Renderer::DrawLine(Point p1, Point p2, Color c)
{
  Point d;
  d = p2 - p1;

  if (d.x == 0 && d.y ==0)
    return;

  if (abs(d.x) > abs(d.y))
  {
    if (p1.x > p2.x)
      Point::Swap(p1, p2);

    FPint y;
    y.i = p1.y<<16;

    int32_t m = ((int32_t)(d.y)<<16)/(d.x);

    Point p;
    for (size_t x = p1.x; x <= p2.x; x++, y.i+=m)
    {
      p.x = x; p.y = y.hi;
      _window.SetPixel(p, c);
    }
  }
  else
  {
    if (p1.y > p2.y)
      Point::Swap(p1, p2);

    FPint x;
    x.i = p1.x<<16;

    int32_t m = ((int32_t)(d.x)<<16)/(d.y);

    Point p;
    for (size_t y = p1.y; y <= p2.y; y++, x.i+=m)
    {
      p.x = x.hi; p.y = y;
      _window.SetPixel(p, c);
    }
  }
}
