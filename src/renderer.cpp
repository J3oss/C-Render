#include <renderer.h>

#include <var.h>

#include <camera.h>

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
  // for (size_t objIndex = 0; objIndex < scene->mObjects.size(); objIndex++) {
  //   DrawObject(scene, objIndex);
  // }
}

void Renderer::DrawObject(std::shared_ptr<Scene> scene, uint32_t _object_index)
{
  // //remove this later
  // Color white(255, 255, 255);
  // Object o = scene->mObjects[_object_index];
  //
  // //draw mesh
  // for (size_t index = 0; index < o.mesh.mIndices.size(); index+=3) {
  //   uint32_t p1Index = o.mesh.mIndices[index];
  //   uint32_t p2Index = o.mesh.mIndices[index+1];
  //   uint32_t p3Index = o.mesh.mIndices[index+2];
  //
  //   scene->mCameras[scene->mActiveCameraIndex]->UpdateView();
  //   scene->mCameras[scene->mActiveCameraIndex]->UpdateVP();
  //
  //   glm::vec4 ss1 = mViewPort * scene->mCameras[scene->mActiveCameraIndex]->mVP * o.mesh.mPositions[p1Index];
  //   glm::vec4 ss2 = mViewPort * scene->mCameras[scene->mActiveCameraIndex]->mVP * o.mesh.mPositions[p2Index];
  //   glm::vec4 ss3 = mViewPort * scene->mCameras[scene->mActiveCameraIndex]->mVP * o.mesh.mPositions[p3Index];
  //
  //   Point p1(ss1.x/ss1.w, ss1.y/ss1.w, ss1.z/ss1.w);
  //   Point p2(ss2.x/ss2.w, ss2.y/ss2.w, ss2.z/ss2.w);
  //   Point p3(ss3.x/ss3.w, ss3.y/ss3.w, ss3.z/ss3.w);
  //
  //   DrawTriangle(p1,
  //                p2,
  //                p3,
  //                white);
  // }
}

void Renderer::DrawTriangle(Point p1, Point p2, Point p3, Color c)
{
  if (p1.x < WIDTH && p1.y < HEIGHT && p2.x < WIDTH && p2.y < HEIGHT)
    DrawLine(p1, p2, c);

  if (p2.x < WIDTH && p2.y < HEIGHT && p3.x < WIDTH && p3.y < HEIGHT)
    DrawLine(p2, p3, c);

  if (p3.x < WIDTH && p3.y < HEIGHT && p1.x < WIDTH && p1.y < HEIGHT)
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

void Renderer::SetViewPort(int x, int y)
{
  mViewPort = glm::mat4(1.0);

  mViewPort[0][0] = WIDTH/2.0f;
  mViewPort[1][1] = HEIGHT/2.0f;
  mViewPort[2][2] = DEPTH/2.0f;

  mViewPort[3][0] = x + WIDTH/2.0f;
  mViewPort[3][1] = y + HEIGHT/2.0f;
  mViewPort[3][2] = DEPTH/2.0f;
  mViewPort[3][3] = 1.0f;
}
