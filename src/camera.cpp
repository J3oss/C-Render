#include <camera.h>

#include <var.h>

Camera::Camera()
{
  mProjection = glm::perspective(45.0f, WIDTH/(float)HEIGHT, 0.1f, 1000.0f);

  UpdateView();
  UpdateVP();
}

Camera::Camera(float fov, float aspect, float near, float far)
{
  mProjection = glm::perspective(fov, aspect, near, far);

  UpdateView();
  UpdateVP();
}

void Camera::UpdateView()
{
  mView = glm::inverse(GetGlobalTransform());
}

void Camera::UpdateVP()
{
  mVP = mProjection * mView;
}

glm::vec3 Camera::GetLookAt()
{
  glm::mat4 transfom = GetGlobalTransform();
  return glm::vec3(transfom[2].x, transfom[2].y, transfom[2].z);
}
