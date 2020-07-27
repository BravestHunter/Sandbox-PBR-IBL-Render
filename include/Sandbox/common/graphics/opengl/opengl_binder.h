#pragma once

#include <string>

#include "Sandbox/utils/operation_result.h"
#include "Sandbox/graphics/graphics_model.h"
#include "Sandbox/common/graphics/opengl/opengl_model.h"
#include "Sandbox/common/graphics/opengl/opengl_hdr_skybox.h"

namespace sandbox_common
{
  class OpenglBinder
  {
   public:
    static sandbox_utils::OperationResult BindModel(OpenglModel &opengl_model, const sandbox_graphics::GraphicsModel model);
    static void UnbindModel(OpenglModel& opengl_model);
    static sandbox_utils::OperationResult BindHdrSkybox(OpenglHdrSkybox &skybox, std::string hdr_filename);
    static void UnbindHdrSkybox(OpenglHdrSkybox &skybox);

   private:
    static void BindMesh(const sandbox_graphics::Mesh &mesh, OpenglMesh &opengl_mesh);
    static int GetOpenglMeshType(sandbox_graphics::PrimitiveType type);
    static int GetOpenglTextureTarget(sandbox_graphics::TextureTarget target);
    static int GetOpenglTextureMinFilterMode(sandbox_graphics::MinFilterMode mode);
    static int GetOpenglTextureMagFilterMode(sandbox_graphics::MagFilterMode mode);
    static int GetOpenglTextureWrapMode(sandbox_graphics::WrapMode mode);
    static void RenderMesh(const OpenglMesh &mesh);
    static void RenderQuad();
  };
}
