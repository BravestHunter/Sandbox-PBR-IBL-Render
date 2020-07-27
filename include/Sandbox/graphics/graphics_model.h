#pragma once

#include "Sandbox/graphics/mesh.h"
#include "Sandbox/graphics/material.h"

namespace sandbox_graphics
{
  enum class ModelType
  {
    GLTF,
    UNKNOWN
  };

  struct GraphicsModel
  {
    ModelType type;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
  };
}
