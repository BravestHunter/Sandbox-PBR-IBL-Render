#pragma once

#include "Sandbox/common/i_module.h"
#include "Sandbox/common/scene/scene_module.h"
#include "Sandbox/common/graphics/opengl/opengl_shader.h"
#include "Sandbox/common/graphics/camera.h"
#include "Sandbox/common/graphics/opengl/opengl_model.h"
#include "Sandbox/common/graphics/opengl/opengl_hdr_skybox.h"

namespace sandbox_common
{
  class OpenglRenderModule : public IModule
  {
   public:
     OpenglRenderModule(unsigned int width, unsigned int height) : IModule(), width_(width), height_(height) {}
    virtual ~OpenglRenderModule() = default;

    virtual void Construct() override;
    virtual void Destruct() override;
    virtual void Tick(float delta_time) override;

    void SetWidth(unsigned int width);
    unsigned int GetWidth();
    void SetHeight(unsigned int height);
    unsigned int GetHeight();
    unsigned int GetPolygonMode();
    void SetPolygonMode(unsigned int mode);
    bool GetLightingSimulationMode();
    void SetLightingSimulationMode(bool mode);
    bool GetPointLightsMode();
    void SetPointLightsMode(bool mode);
    bool GetSkyboxUsingMode();
    void SetSkyboxUsingMode(bool mode);

    Entity GetSkyboxEntity();
    void SetSkyboxEntity(Entity entity);

    std::weak_ptr<Camera> GetCamera();

  private:
    unsigned int width_;
    unsigned int height_;
    unsigned int polygon_mode_ = GL_FILL;
    bool use_lighting_simulation_ = true;
    bool use_point_lights = false;
    bool use_skybox_ = false;

    Entity skybox_entity_;
    OpenglHdrSkybox *skybox_;

    std::shared_ptr<Camera> camera_;
    OpenglShader model_shader_;
    OpenglShader skybox_shader_;

    void DrawModel(const OpenglModel &model, bool transparency = false);
    void DrawSkybox();
    std::string GetTextureTypeName(sandbox_graphics::TextureType type);
  };
}