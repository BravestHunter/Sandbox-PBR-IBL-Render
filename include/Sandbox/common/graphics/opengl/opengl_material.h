#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Sandbox/common/graphics/opengl/opengl_texture.h"
#include "Sandbox/graphics/material.h"

namespace sandbox_common
{
  struct OpenglMaterial
  {
    std::string name;

    glm::vec4 base_color;
    float metallicity;
    float roughness;
    float occlusion;
    glm::vec4 emissive;
    std::vector<OpenglTexture> textures;

    sandbox_graphics::AlphaMode alpha_mode;
    float alpha_cutoff;
    bool double_sided;
  };
}
