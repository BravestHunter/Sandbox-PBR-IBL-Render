#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

namespace sandbox_common
{
  enum class CameraMovement 
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  const float YAW_DEFAULT = -90.0f;
  const float PITCH_DEFAULT = 0.0f;
  const float ZOOM_DEFAULT = 45.0f;

  class Camera
  {
   public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
      float yaw = YAW_DEFAULT, float pitch = PITCH_DEFAULT, float zoom = ZOOM_DEFAULT)
      : yaw_(yaw), pitch_(pitch), zoom_(zoom)
    {
      position_ = position;
      world_up_ = up;
      UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix();

    void ChangePosition(CameraMovement direction, float deltaTime);
    void ChangeDirection(float xoffset, float yoffset, bool constrainPitch = true);
    void ChangeZoom(float offset);

    glm::vec3 GetPosition() const;
    void SetPosition(glm::vec3 position);
    float GetZoom() const;
    void SetZoom(float zoom);

   private:
    glm::vec3 position_;
    float yaw_;
    float pitch_;
    float zoom_;

    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    void UpdateCameraVectors();
  };
}
