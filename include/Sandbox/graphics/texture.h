#pragma once

#include <string>

#include "Sandbox/utils/image.h"

namespace sandbox_graphics
{
  enum class TextureType
  {
    BASE_COLOR,
    METALLIC_ROUGHNESS,
    NORMAL,
    OCCLUSION,
    EMISSIVE,
    UNKNOWN,
  };

  enum class TextureTarget
  {
    _1D,
    _2D,
    _3D,
    UNKNOWN
  };

  enum class MinFilterMode
  {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR,
    UNKNOWN
  };

  enum class MagFilterMode
  {
    NEAREST,
    LINEAR,
    UNKNOWN
  };

  enum class WrapMode
  {
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRRORED_REPEAT,
    REPEAT,
    UNKNOWN
  };

  struct Texture
  {
    TextureType type;
    TextureTarget target;
    MinFilterMode min_filter_mode;
    MagFilterMode mag_filter_mode;
    WrapMode wrap_s_mode;
    WrapMode wrap_t_mode;

    sandbox_utils::Image image;
  };
}
