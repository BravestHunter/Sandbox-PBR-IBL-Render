#pragma once

#include <string>

#include "Sandbox/utils/operation_result.h"
#include "Sandbox/graphics/graphics_model.h"

namespace sandbox_graphics
{
  class ModelLoader
  {
   public:
    static sandbox_utils::OperationResult LoadGltfModel(GraphicsModel &model, const std::string filename);

    //gltf models loading
    static PrimitiveType GetGltfPrimitiveType(int type);
    static void GenerateTangents(Mesh &mesh);
    static void GenerateTangent(Vertex &vertex1, Vertex &vertex2, Vertex &vertex3);
    static MinFilterMode GetGltfMinFilterMode(int mode);
    static MagFilterMode GetGltfMagFilterMode(int mode);
    static WrapMode GetGltfWrapMode(int mode);
    static AlphaMode GetGltfAlphaMode(const std::string mode);
  };
}
