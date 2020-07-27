#pragma once

#include "Sandbox/graphics/mesh.h"

namespace sandbox_graphics
{
  class BasicMeshes
  {
   public:
    static Mesh Cube();
    static Mesh ReversedCube();
    static Mesh Axes();
  };
}
