#include <renderer.h>

#include <var.h>

#include <camera.h>

#define SWAP(x,y) { glm::ivec2 temp = x; x = y; y = temp;}
#define CLAMP(x,lo,hi) { x = x < lo ? lo : x > hi ? hi : x}

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
  zBuffer = new float[WIDTH*HEIGHT];
  for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
    zBuffer[i] = std::numeric_limits<float>::max();
  }
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
  scene->mCameras[scene->mActiveCameraIndex]->UpdateView();
  scene->mCameras[scene->mActiveCameraIndex]->UpdateVP();

  for (size_t meshIndex = 0; meshIndex < scene->mMeshes.size(); meshIndex++) {
    DrawMesh(scene, meshIndex);
  }
}

glm::vec3 lightdir(-1,-2,-3);
void Renderer::DrawMesh(std::shared_ptr<Scene> scene, uint32_t meshIndex)
{
  auto m = scene->mMeshes[meshIndex];
  glm::mat4 vvp = mViewPort * scene->mCameras[scene->mActiveCameraIndex]->mVP;
  glm::vec3 cLookAt = scene->mCameras[scene->mActiveCameraIndex]->GetLookAt();

  //draw mesh
  for (size_t index = 0; index < m->mIndices.size(); index+=3)
  {
    uint32_t p1Index = m->mIndices[index];
    uint32_t p2Index = m->mIndices[index+1];
    uint32_t p3Index = m->mIndices[index+2];

    glm::vec4 wc1 = m->GetGlobalTransform() * m->mPositions[p1Index];
    glm::vec4 wc2 = m->GetGlobalTransform() * m->mPositions[p2Index];
    glm::vec4 wc3 = m->GetGlobalTransform() * m->mPositions[p3Index];

    glm::vec4 cc1 = vvp * wc1;
    glm::vec4 cc2 = vvp * wc2;
    glm::vec4 cc3 = vvp * wc3;

    glm::vec3 surfaceNormal = glm::normalize(cross( glm::vec3(wc3.x,wc3.y,wc3.z)-glm::vec3(wc1.x,wc1.y,wc1.z), glm::vec3(wc2.x,wc2.y,wc2.z)-glm::vec3(wc1.x,wc1.y,wc1.z)));

    lightdir = glm::normalize(lightdir);

    if ( glm::dot(surfaceNormal, cLookAt) < 0)
    {
      float intensity = glm::dot(surfaceNormal, lightdir);
      intensity = CLAMP(intensity, 0, 255);

      // glm::ivec3 p1(cc1.x/cc1.w, cc1.y/cc1.w, 1);
      // glm::ivec3 p2(cc2.x/cc2.w, cc2.y/cc2.w, 1);
      // glm::ivec3 p3(cc3.x/cc3.w, cc3.y/cc3.w, 1);

      glm::ivec3 x(cc1.x/cc1.w, cc2.x/cc2.w, cc3.x/cc3.w);
      glm::ivec3 y(cc1.y/cc1.w, cc2.y/cc2.w, cc3.y/cc3.w);
      glm::vec3  z(cc1.z/cc1.w, cc2.z/cc2.w, cc3.z/cc3.w);

      // DrawTriangle(p1, p2, p3);

      DrawTriangleSolid(x, y, z, intensity);
    }
  }
}

void Renderer::DrawTriangle(glm::ivec3 p1, glm::ivec3 p2, glm::ivec3 p3)
{
  Color white(255, 255, 255);

  if (p1.x < WIDTH && p1.y < HEIGHT && p2.x < WIDTH && p2.y < HEIGHT)
    DrawLine(p1, p2, white);

  if (p2.x < WIDTH && p2.y < HEIGHT && p3.x < WIDTH && p3.y < HEIGHT)
    DrawLine(p2, p3, white);

  if (p3.x < WIDTH && p3.y < HEIGHT && p1.x < WIDTH && p1.y < HEIGHT)
    DrawLine(p3, p1, white);
}

glm::vec3 barycentric(glm::ivec3 x, glm::ivec3 y, glm::ivec2 p)
{
    glm::vec3 u = cross( glm::vec3(x[2]-x[0], x[1]-x[0], x[0]-p.x), glm::vec3(y[2]-y[0], y[1]-y[0], y[0]-p.y));

    if (std::abs(u.z)<1) return glm::vec3(-1,1,1);

    return glm::vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
}

void Renderer::DrawTriangleSolid(glm::ivec3 x, glm::ivec3 y, glm::vec3 z, float intensity)
{
  glm::ivec2 bBoxMin, bBoxMax;

  bBoxMax.x = std::min(  WIDTH, std::max( x[0], std::max( x[1] ,x[2] ) ) );
  bBoxMax.y = std::min( HEIGHT, std::max( y[0], std::max( y[1] ,y[2] ) ) );

  bBoxMin.x = std::max(      0, std::min( x[0], std::min( x[1] ,x[2]) ) );
  bBoxMin.y = std::max(      0, std::min( y[0], std::min( y[1] ,y[2]) ) );

  float pz;
  glm::ivec2 p;
  for (p.x = bBoxMin.x; p.x < bBoxMax.x; p.x++)
  {
    for (p.y = bBoxMin.y; p.y < bBoxMax.y; p.y++)
    {
      glm::vec3 bc = barycentric(x, y, p);
      pz = z[0]*bc[0] + z[1]*bc[1] + z[2]*bc[2];
      if (bc.x<0 || bc.y<0 || bc.z<0 || zBuffer[p.x + p.y * WIDTH] < pz) continue;
      zBuffer[p.x + p.y * WIDTH] = pz;
      Color White(intensity * 255, intensity * 255, intensity * 255);
      _window.SetPixel(p.x, p.y, White);
    }
  }
}

void Renderer::DrawLine(glm::ivec2 p1, glm::ivec2 p2, Color c)
{
  auto d = p2 - p1;

  if (d.x == 0 && d.y == 0)
    return;

  if (std::abs(d.x) > std::abs(d.y))
  {
    if (p1.x > p2.x)
      SWAP(p1, p2);

    FPint y;
    y.i = p1.y<<16;

    int32_t m = ((int32_t)(d.y)<<16)/(d.x);

    glm::ivec2 p;
    for (size_t x = p1.x; x <= p2.x; x++, y.i+=m)
    {
      p.x = x; p.y = y.hi;
      _window.SetPixel(p.x, p.y, c);
    }
  }
  else
  {
    if (p1.y > p2.y)
      SWAP(p1, p2);

    FPint x;
    x.i = p1.x<<16;

    int32_t m = ((int32_t)(d.x)<<16)/(d.y);

    glm::ivec2 p;
    for (size_t y = p1.y; y <= p2.y; y++, x.i+=m)
    {
      p.x = x.hi; p.y = y;
      _window.SetPixel(p.x, p.y, c);
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
