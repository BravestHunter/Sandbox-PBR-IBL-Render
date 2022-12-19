#include "Sandbox/common/graphics/opengl/opengl_render_module.h"

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)
#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

#include "resourceConfig.h"

#include <tuple>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Sandbox/platform/platform_provider.h"
#include "Sandbox/common/graphics/opengl/opengl_model_component.h"
#include "Sandbox/graphics/point_light.h"

namespace sandbox_common
{
  void OpenglRenderModule::Construct()
  {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK)
    {
      // log error
      return;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera_ = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));

    model_shader_ = OpenglShader(PPCAT(SHADERS_DIR, MODEL_VERTEX_SHADER), PPCAT(SHADERS_DIR, MODEL_FRAGMENT_SHADER));
    skybox_shader_ = OpenglShader(PPCAT(SHADERS_DIR, SKYBOX_VERTEX_SHADER), PPCAT(SHADERS_DIR, SKYBOX_FRAGMENT_SHADER));

    is_constructed_ = true;
  }

  void OpenglRenderModule::Destruct()
  {
    is_constructed_ = false;
  }

  void OpenglRenderModule::Tick(float delta_time)
  {
    if (parent_.expired())
      return;

    std::shared_ptr<SceneModule> scene = std::static_pointer_cast<SceneModule>(parent_.lock());

    std::weak_ptr<sandbox_platform::IPlatformWindow> window = sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow();
    if (window.expired())
      return;

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (use_skybox_)
    {
      skybox_ = scene->GetComponent<OpenglHdrSkybox>(skybox_entity_);
    }
    
    glm::mat4 projection = glm::perspective(glm::radians(camera_->GetZoom()), (float)width_ / (float)height_, 0.1f, 100.0f);
    glm::mat4 view = camera_->GetViewMatrix();

    model_shader_.Use();
    model_shader_.SetVec3("VIEW_POSITION", camera_->GetPosition());
    model_shader_.SetMat4("PROJECTION", projection);
    model_shader_.SetMat4("VIEW", view);
    
    if (use_point_lights)
    {
      ComponentStorage<sandbox_graphics::PointLight>::StorageItem<sandbox_graphics::PointLight>* light_components_ref = nullptr;
      size_t light_component_count;
      if (scene->GetComponents<sandbox_graphics::PointLight>(&light_components_ref, &light_component_count) == sandbox_utils::OperationResult::SUCCESS)
      {
        for (size_t i = 0; i < light_component_count; i++)
        {
          if (light_components_ref->exist)
          {
            model_shader_.SetVec3("POINT_LIGHTS[" + std::to_string(i) + "].position", light_components_ref->data.position);
            model_shader_.SetVec3("POINT_LIGHTS[" + std::to_string(i) + "].color", light_components_ref->data.color);
          }
          light_components_ref++;
        }
      }
    }
    else
    {
      model_shader_.SetVec3("POINT_LIGHTS[0].color", glm::vec3(0, 0, 0));
      model_shader_.SetVec3("POINT_LIGHTS[1].color", glm::vec3(0, 0, 0));
    }
    
    // load models
    ComponentStorage<OpenglModelComponent>::StorageItem<OpenglModelComponent>* gl_model_components_ref = nullptr;
    size_t gl_model_component_count;
    sandbox_utils::OperationResult load_models_result = scene->GetComponents<OpenglModelComponent>(&gl_model_components_ref, &gl_model_component_count);

    // draw non transparent meshes
    if (load_models_result == sandbox_utils::OperationResult::SUCCESS)
    {
      for (size_t i = 0; i < gl_model_component_count; i++)
      {
        if (gl_model_components_ref->exist)
        {
          glm::mat4 translate = glm::translate(glm::mat4(1.0), gl_model_components_ref->data.transformation.position);
          glm::mat4 rotate = glm::toMat4(gl_model_components_ref->data.transformation.rotation);
          glm::mat4 scale = glm::scale(glm::mat4(1.0), gl_model_components_ref->data.transformation.scale);
    
          model_shader_.SetMat4("MODEL", translate * rotate * scale);
          DrawModel(gl_model_components_ref->data.model, false);
        }
        gl_model_components_ref++;
      }
    }
    
    // draw skybox
    if (use_skybox_)
    {
      view = glm::mat4(glm::mat3(view));

      skybox_shader_.Use();
      skybox_shader_.SetMat4("PROJECTION", projection);
      skybox_shader_.SetMat4("VIEW", view);
    
      DrawSkybox();
    }

    // draw transparent meshes
    model_shader_.Use();
    if (load_models_result == sandbox_utils::OperationResult::SUCCESS)
    {
      gl_model_components_ref -= gl_model_component_count;
      for (size_t i = 0; i < gl_model_component_count; i++)
      {
        if (gl_model_components_ref->exist)
        {
          glm::mat4 translate = glm::translate(glm::mat4(1.0), gl_model_components_ref->data.transformation.position);
          glm::mat4 rotate = glm::toMat4(gl_model_components_ref->data.transformation.rotation);
          glm::mat4 scale = glm::scale(glm::mat4(1.0), gl_model_components_ref->data.transformation.scale);
    
          model_shader_.SetMat4("MODEL", translate * rotate * scale);
          DrawModel(gl_model_components_ref->data.model, true);
        }
        gl_model_components_ref++;
      }
    }

    window.lock()->SwapBuffers();
  }

  void OpenglRenderModule::SetWidth(unsigned int width)
  {
    width_ = width;
  }

  unsigned int OpenglRenderModule::GetWidth()
  {
    return width_;
  }

  void OpenglRenderModule::SetHeight(unsigned int height)
  {
    height_ = height;
  }

  unsigned int OpenglRenderModule::GetHeight()
  {
    return height_;
  }

  unsigned int OpenglRenderModule::GetPolygonMode()
  {
    return polygon_mode_;
  }

  void OpenglRenderModule::SetPolygonMode(unsigned int mode)
  {
    if (mode != GL_POINT && mode != GL_LINE && mode != GL_FILL)
      return;

    polygon_mode_ = mode;
    glPolygonMode(GL_FRONT_AND_BACK, mode);
  }

  bool OpenglRenderModule::GetLightingSimulationMode()
  {
    return use_lighting_simulation_;
  }

  void OpenglRenderModule::SetLightingSimulationMode(bool mode)
  {
    use_lighting_simulation_ = mode;
  }

  bool OpenglRenderModule::GetPointLightsMode()
  {
    return use_point_lights;
  }

  void OpenglRenderModule::SetPointLightsMode(bool mode)
  {
    use_point_lights = mode;
  }

  bool OpenglRenderModule::GetSkyboxUsingMode()
  {
    return use_skybox_;
  }

  void OpenglRenderModule::SetSkyboxUsingMode(bool mode)
  {
    use_skybox_ = mode;
  }

  Entity OpenglRenderModule::GetSkyboxEntity()
  {
    return skybox_entity_;
  }

  void OpenglRenderModule::SetSkyboxEntity(Entity entity)
  {
    skybox_entity_ = entity;
  }

  std::weak_ptr<Camera> OpenglRenderModule::GetCamera()
  {
    return camera_;
  }

  void OpenglRenderModule::DrawModel(const OpenglModel &model, bool transparency)
  {
    for (OpenglMesh mesh : model.meshes)
    {
      if (mesh.material > -1)
      {
        const OpenglMaterial material = model.materials[mesh.material];

        if ((transparency && material.alpha_mode == sandbox_graphics::AlphaMode::OPAQUe) || (!transparency && material.alpha_mode != sandbox_graphics::AlphaMode::OPAQUe))
          continue;

        model_shader_.SetVec4("BASE_COLOR", material.base_color);
        model_shader_.SetFloat("METALLICITY", material.metallicity);
        model_shader_.SetFloat("ROUGHNESS", material.roughness);
        model_shader_.SetFloat("OCCLUSION", material.occlusion);
        model_shader_.SetVec4("EMISSIVE", material.emissive);

        bool use_base_color_texture = false;
        bool use_metallic_roughness_texture = false;
        bool use_normal_texture = false;
        bool use_occlusion_texture = false;
        bool use_emissive_texture = false;
        for (unsigned int i = 0; i < material.textures.size(); i++)
        {
          switch (material.textures[i].type)
          {
          case sandbox_graphics::TextureType::BASE_COLOR:
            use_base_color_texture = true;
            break;
          case sandbox_graphics::TextureType::METALLIC_ROUGHNESS:
            use_metallic_roughness_texture = true;
            break;
          case sandbox_graphics::TextureType::NORMAL:
            use_normal_texture = true;
            break;
          case sandbox_graphics::TextureType::OCCLUSION:
            use_occlusion_texture = true;
            break;
          case sandbox_graphics::TextureType::EMISSIVE:
            use_emissive_texture = true;
            break;
          }

          glActiveTexture(GL_TEXTURE0 + i);

          glUniform1i(glGetUniformLocation(model_shader_.GetId(), GetTextureTypeName(material.textures[i].type).c_str()), i);
          glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
        }

        if (use_skybox_)
        {
          size_t shift = mesh.material > -1 ? model.materials[mesh.material].textures.size() : 0;

          glActiveTexture(GL_TEXTURE0 + shift);
          glUniform1i(glGetUniformLocation(model_shader_.GetId(), "IRRADIANCE_MAP"), shift);
          glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_->irradiance_map);

          glActiveTexture(GL_TEXTURE0 + shift + 1);
          glUniform1i(glGetUniformLocation(model_shader_.GetId(), "PREFILTER_MAP"), shift + 1);
          glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_->prefilter_map);

          glActiveTexture(GL_TEXTURE0 + shift + 2);
          glUniform1i(glGetUniformLocation(model_shader_.GetId(), "TEXTURE_BRDF_LUT"), shift + 2);
          glBindTexture(GL_TEXTURE_2D, skybox_->brdf_lut_map);
        }

        glActiveTexture(GL_TEXTURE0);

        model_shader_.SetBool("USE_BASE_COLOR_TEXTURE", use_base_color_texture);
        model_shader_.SetBool("USE_METALLIC_ROUGHNESS_TEXTURE", use_metallic_roughness_texture);
        model_shader_.SetBool("USE_NORMAL_TEXTURE", use_normal_texture);
        model_shader_.SetBool("USE_OCCLUSION_TEXTURE", use_occlusion_texture);
        model_shader_.SetBool("USE_EMISSIVE_TEXTURE", use_emissive_texture);

        model_shader_.SetBool("USE_IBL", use_skybox_);

        model_shader_.SetBool("USE_LIGHTING_SIMULATION", use_lighting_simulation_);
      }
      else if (transparency)
      {
        continue;
      }

      glBindVertexArray(mesh.vao);
      if (mesh.indices_used)
      {
        glDrawElements(mesh.type, mesh.indeces_count, GL_UNSIGNED_INT, 0);
      }
      else
      {
        glDrawArrays(mesh.type, 0, mesh.vertices_count);
      }

      glBindVertexArray(0);
    }
  }

  void OpenglRenderModule::DrawSkybox()
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_->cubemap);
    
    glBindVertexArray(skybox_->cube_mesh.vao);
    glDrawElements(skybox_->cube_mesh.type, skybox_->cube_mesh.indeces_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  std::string OpenglRenderModule::GetTextureTypeName(sandbox_graphics::TextureType type)
  {
    switch (type)
    {
    case sandbox_graphics::TextureType::BASE_COLOR:
      return "TEXTURE_BASE_COLOR";
    case sandbox_graphics::TextureType::METALLIC_ROUGHNESS:
      return "TEXTURE_METALLIC_ROUGHNESS";
    case sandbox_graphics::TextureType::NORMAL:
      return "TEXTURE_NORMAL";
    case sandbox_graphics::TextureType::OCCLUSION:
      return "TEXTURE_OCCLUSION";
    case sandbox_graphics::TextureType::EMISSIVE:
      return "TEXTURE_EMISSIVE";
    default:
      return "TEXTURE_UNKNOWN";
    }
  }
}
