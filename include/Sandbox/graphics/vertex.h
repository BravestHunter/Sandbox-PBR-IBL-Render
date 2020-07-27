#pragma once

#include <glm/glm.hpp>

#include "Sandbox/utils/color.h"

namespace sandbox_graphics
{
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 tangent;

    glm::vec2 texture_coords;
    glm::vec4 color;
    //sandbox_utils::Color color;
  };
}
