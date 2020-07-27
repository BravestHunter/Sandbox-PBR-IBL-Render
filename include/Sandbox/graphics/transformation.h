#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

namespace sandbox_graphics
{
  struct Transformation
  {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
  };
}
