#pragma once

#include <vector>

#include "Sandbox/graphics/vertex.h"

namespace sandbox_graphics
{
  enum class PrimitiveType
  {
    POINT,
    LINE,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLE,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    QUAD,
    QUAD_STRIP,
    POLYGON,
    UNKNOWN
  };

  struct Mesh
  {
    PrimitiveType type;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int material;

    bool indices_used;
  };
}
