#include <renderer.h>

#include <var.h>
#include <camera.h>

#include <ext/glm/glm/gtx/scalar_multiplication.hpp>

// #include <shaders/flat_shader.h>
// FlatShader sh;

// #include <shaders/gouraud_shader.h>
// GouraudShader sh;

#include <shaders/phong_shader.h>
PhongShader sh;

// #include <shaders/ggshader.h>
// FlatShader sh;

#define SWAP(x,y) { glm::ivec2 temp = x; x = y; y = temp;}

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

  zBuffer.resize(WIDTH*HEIGHT);
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
  for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
    zBuffer[i] = std::numeric_limits<float>::max();
  }
}

void Renderer::DrawScene(std::shared_ptr<Scene> scene)
{
  scene->mCameras[scene->mActiveCameraIndex]->UpdateView();
  scene->mCameras[scene->mActiveCameraIndex]->UpdateVP();

  for (size_t meshIndex = 0; meshIndex < scene->mMeshes.size(); meshIndex++) {
    DrawMesh(scene, meshIndex);
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

void Renderer::SetDepthTest(DepthTest test)
{
  mDepthTest = test;
}

void Renderer::SetCullMode(CullingMode mode)
{
  mCullMode = mode;
}

glm::vec3 barycentric(glm::ivec3 x, glm::ivec3 y, glm::ivec2 p)
{
    glm::vec3 u = cross( glm::vec3(x[2]-x[0], x[1]-x[0], x[0]-p.x), glm::vec3(y[2]-y[0], y[1]-y[0], y[0]-p.y));

    if (std::abs(u.z)<1) return glm::vec3(-1,1,1);

    return glm::vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
}

void Renderer::DrawMesh(std::shared_ptr<Scene> scene, uint32_t meshIndex)
{
  auto m = scene->mMeshes[meshIndex];

  sh.inPositions = m->mPositions;
  sh.inNormals = m->mNormals;
  sh.inTangents = m->mTangets;
  sh.inUVs = m->mUVs;

  sh.inLights = scene->mLights;
  sh.inMaterial = m->mMaterial;
  sh.inCamera = scene->mCameras[scene->mActiveCameraIndex];

  sh.mvpMatrix = scene->mCameras[scene->mActiveCameraIndex]->mVP * m->GetGlobalTransform();
  sh.modelMatrix = m->GetGlobalTransform();
  sh.normalMatrix = glm::transpose(glm::inverse(glm::mat3(m->GetGlobalTransform())));

  // #pragma omp parallel for
  for (size_t index = 0; index < m->mIndices.size(); index+=3)
  {
    uint32_t indecies[3] = {m->mIndices[index+0], m->mIndices[index+1], m->mIndices[index+2]};

    sh.vertex(indecies);

    float det = sh.outPositions[0].x * (sh.outPositions[1].y - sh.outPositions[2].y) +
                sh.outPositions[1].x * (sh.outPositions[2].y - sh.outPositions[0].y) +
                sh.outPositions[2].x * (sh.outPositions[0].y - sh.outPositions[1].y);

    if ( (mCullMode == CullingMode::NONE) ||
         (mCullMode == CullingMode::BACK && det > 0) ||
         (mCullMode == CullingMode::FRONT && det < 0) )
    {
      glm::vec4 cc1 = mViewPort * sh.outPositions[0];
      glm::vec4 cc2 = mViewPort * sh.outPositions[1];
      glm::vec4 cc3 = mViewPort * sh.outPositions[2];

      glm::ivec3 x(cc1.x/cc1.w, cc2.x/cc2.w, cc3.x/cc3.w);
      glm::ivec3 y(cc1.y/cc1.w, cc2.y/cc2.w, cc3.y/cc3.w);
      glm::vec3  z(cc1.z/cc1.w, cc2.z/cc2.w, cc3.z/cc3.w);

      glm::ivec2 bBoxMin, bBoxMax;

      bBoxMax.x = std::min(  WIDTH, std::max( x[0], std::max( x[1] ,x[2] ) ) );
      bBoxMax.y = std::min( HEIGHT, std::max( y[0], std::max( y[1] ,y[2] ) ) );

      bBoxMin.x = std::max(      0, std::min( x[0], std::min( x[1] ,x[2]) ) );
      bBoxMin.y = std::max(      0, std::min( y[0], std::min( y[1] ,y[2]) ) );

      float pz;
      glm::ivec2 p;
      for (p.x = bBoxMin.x; p.x < bBoxMax.x; p.x++)
        for (p.y = bBoxMin.y; p.y < bBoxMax.y; p.y++)
        {
          glm::vec3 bc = barycentric(x, y, p);
          pz = z[0]*bc[0] + z[1]*bc[1] + z[2]*bc[2];
          if (bc.x<0 || bc.y<0 || bc.z<0 || zBuffer[p.x + p.y * WIDTH] < pz) continue;

          zBuffer[p.x + p.y * WIDTH] = pz;

          Color c = sh.fragment(bc);

          _window.SetPixel(p.x, p.y, c);
        }
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
