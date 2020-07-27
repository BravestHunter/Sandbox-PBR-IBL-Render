#pragma once

#include "Sandbox/graphics/texture.h"

namespace sandbox_common
{
  struct OpenglTexture
  {
    sandbox_graphics::TextureType type;
    unsigned int id;
  };
}
