#include <camera.h>

#include <var.h>

Camera::Camera()
{
  mProjection = glm::perspective(45.0f, WIDTH/(float)HEIGHT, 0.1f, 1000.0f);

  mWorld_Tranform = glm::mat4(1.0);
  mWorld_Tranform[2].z *= -1.0f;

  UpdateView();
  UpdateVP();
}

Camera::Camera(float fov, float aspect, float near, float far)
{
  mProjection = glm::perspective(fov, aspect, near, far);

  mWorld_Tranform = glm::mat4(1.0);
  mWorld_Tranform[2].z *= -1.0f; //camera forward is the -z

  UpdateView();
  UpdateVP();
}

void Camera::UpdateView()
{
  mView = glm::inverse(mWorld_Tranform);
}

void Camera::UpdateVP()
{
  mVP = mProjection * mView;
}
