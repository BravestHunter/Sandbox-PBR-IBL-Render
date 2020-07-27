#include "Sandbox/graphics/basic_meshes.h"

namespace sandbox_graphics
{
  Mesh BasicMeshes::Cube()
  {
    return Mesh({
      PrimitiveType::TRIANGLE,
      std::vector<Vertex>{
        // top
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // bot
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // right
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // left
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // front
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // top
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
      },
      std::vector<unsigned int>{
          0, 1, 2,
          3, 2, 1,
          6, 5, 4,
          5, 6, 7,
          8, 9, 10,
          11, 10, 9,
          14, 13, 12,
          13, 14, 15,
          16, 17, 18,
          19, 18, 17,
          22, 21, 20,
          21, 22, 23
      },
      -1,
      true
    });
  }

  Mesh BasicMeshes::ReversedCube()
  {
    return Mesh({
      PrimitiveType::TRIANGLE,
      std::vector<Vertex>{
        // top
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // bot
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // right
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // left
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1.0f, 0), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // front
        Vertex({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),

        // top
        Vertex({glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
        Vertex({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1.0f), glm::vec4(0, 0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 0, 0, 0)}),
      },
      std::vector<unsigned int>{
          2, 1, 0,
          1, 2, 3,
          4, 5, 6,
          7, 6, 5,
          10, 9, 8,
          9, 10, 11,
          12, 13, 14,
          15, 14, 13,
          18, 17, 16,
          17, 18, 19,
          20, 21, 22,
          23, 22, 21
      },
      -1,
      true
    });
  }

  Mesh BasicMeshes::Axes()
  {
    float length = 1000.0f; // todo: ???

    sandbox_graphics::Mesh mesh;
    mesh.type = sandbox_graphics::PrimitiveType::LINE;
    mesh.vertices.resize(6);
    mesh.vertices[0].position = glm::vec3(length, 0.0f, 0.0f);
    mesh.vertices[1].position = glm::vec3(-length, 0.0f, 0.0f);
    mesh.vertices[2].position = glm::vec3(0.0f, length, 0.0f);
    mesh.vertices[3].position = glm::vec3(0.0f, -length, 0.0f);
    mesh.vertices[4].position = glm::vec3(0.0f, 0.0f, length);
    mesh.vertices[5].position = glm::vec3(0.0f, 0.0f, -length);
    mesh.vertices[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    mesh.vertices[1].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    mesh.vertices[2].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    mesh.vertices[3].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    mesh.vertices[4].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    mesh.vertices[5].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    mesh.indices_used = false;
    mesh.material = -1;

    return mesh;
  }
}
