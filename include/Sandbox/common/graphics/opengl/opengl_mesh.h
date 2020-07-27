#pragma once

#include <vector>

namespace sandbox_common
{
  struct OpenglMesh
  {
    int type;
    int material;

    unsigned int vao;
    unsigned int vbo;
    int vertices_count;

    bool indices_used;
    unsigned int ebo;
    int indeces_count;
  };
}
