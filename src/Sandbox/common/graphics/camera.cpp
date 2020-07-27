#include "Sandbox/common/graphics/camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sandbox_common
{
  glm::mat4 Camera::GetViewMatrix()
  {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  void Camera::ChangePosition(CameraMovement direction, float shift)
  {
    if (direction == CameraMovement::FORWARD)
      position_ += front_ * shift;
    if (direction == CameraMovement::BACKWARD)
      position_ -= front_ * shift;
    if (direction == CameraMovement::LEFT)
      position_ -= right_ * shift;
    if (direction == CameraMovement::RIGHT)
      position_ += right_ * shift;
    if (direction == CameraMovement::UP)
      position_ += up_ * shift;
    if (direction == CameraMovement::DOWN)
      position_ -= up_ * shift;
  }

  void Camera::ChangeDirection(float xoffset, float yoffset, bool constrainPitch)
  {
    yaw_ += xoffset;
    pitch_ += yoffset;

    if (constrainPitch)
    {
      if (pitch_ > 89.0f)
        pitch_ = 89.0f;
      if (pitch_ < -89.0f)
        pitch_ = -89.0f;
    }

    UpdateCameraVectors();
  }

  void Camera::ChangeZoom(float offset)
  {
    if (zoom_ >= 1.0f && zoom_ <= 45.0f)
      zoom_ -= offset;
    if (zoom_ <= 1.0f)
      zoom_ = 1.0f;
    if (zoom_ >= 45.0f)
      zoom_ = 45.0f;
  }

  glm::vec3 Camera::GetPosition() const
  {
    return position_;
  }

  void Camera::SetPosition(glm::vec3 position)
  {
    position_ = position;
  }

  float Camera::GetZoom() const
  {
    return zoom_;
  }

  void Camera::SetZoom(float zoom)
  {
    zoom_ = zoom;
  }

  void Camera::UpdateCameraVectors()
  {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }
}
