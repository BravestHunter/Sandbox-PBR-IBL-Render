#pragma once

#include <string>
#include <vector>

#include "Sandbox/common/graphics/opengl/opengl_mesh.h"
#include "Sandbox/common/graphics/opengl/opengl_material.h"

namespace sandbox_common
{
  struct OpenglModel
  {
    std::string name;
    std::vector<OpenglMesh> meshes;
    std::vector<OpenglMaterial> materials;
  };
}
