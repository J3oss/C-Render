#pragma once

#include <ext/glm/glm/gtc/matrix_transform.hpp>

#include <node.h>

class Camera : public Node {
public:
  glm::mat4 mVP;

  glm::mat4 mProjection;
  glm::mat4 mView;

  Camera();
  Camera(float fov, float aspect, float near, float far);

  void UpdateView();

  void UpdateVP();

  glm::vec3 GetLookAt();
};
