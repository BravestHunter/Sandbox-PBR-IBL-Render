#pragma once

#include "Sandbox/common/graphics/opengl/opengl_mesh.h"

namespace sandbox_common
{
  struct OpenglHdrSkybox
  {
    unsigned int cubemap;
    unsigned int irradiance_map;
    unsigned int prefilter_map;
    unsigned int brdf_lut_map;

    OpenglMesh cube_mesh;
  };
}
