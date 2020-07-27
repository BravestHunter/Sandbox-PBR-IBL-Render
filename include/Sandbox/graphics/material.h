#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Sandbox/graphics/texture.h"

namespace sandbox_graphics
{
  enum class AlphaMode
  {
    OPAQUe,
    BLEND,
    MASK,
  };

  struct Material
  {
    std::string name;

    glm::vec4 base_color;
    double metallicity;
    double roughness;
    double occlusion;
    glm::vec4 emissive;
    std::vector<Texture> textures;

    AlphaMode alpha_mode;
    double alpha_cutoff;
    bool double_sided;
  };
}
