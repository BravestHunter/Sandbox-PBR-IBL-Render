#pragma once

#include "Sandbox/common/graphics/opengl/opengl_model.h"
#include "Sandbox/graphics/transformation.h"

namespace sandbox_common
{
  struct OpenglModelComponent
  {
    OpenglModel model;
    sandbox_graphics::Transformation transformation;
  };
}
