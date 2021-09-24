#pragma once

#include <ext/glm/glm/glm.hpp>
#include <ext/glm/glm/gtc/matrix_transform.hpp>

class Camera {
public:
  glm::mat4 mWorld_Tranform;

  glm::mat4 mVP;

  glm::mat4 mProjection;
  glm::mat4 mView;

  Camera();
  Camera(float fov, float aspect, float near, float far);

  void UpdateView();

  void UpdateVP();
};
