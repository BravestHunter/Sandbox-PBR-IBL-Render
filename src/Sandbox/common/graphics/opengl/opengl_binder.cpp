#include "Sandbox/common/graphics/opengl/opengl_binder.h"

#include <vector>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox/file/file_manager_provider.h"
#include "Sandbox/platform/platform_provider.h"
#include "Sandbox/utils/util_functions.h"
#include "Sandbox/graphics/vertex.h"
#include "Sandbox/graphics/basic_meshes.h"
#include "Sandbox/common/graphics/opengl/opengl_shader.h"

namespace sandbox_common
{
  sandbox_utils::OperationResult OpenglBinder::BindModel(OpenglModel &opengl_model, const sandbox_graphics::GraphicsModel model)
  {
    opengl_model.meshes.resize(model.meshes.size());
    for (size_t i = 0; i < model.meshes.size(); i++)
    {
      const sandbox_graphics::Mesh &mesh = model.meshes[i];
      OpenglMesh &opengl_mesh = opengl_model.meshes[i];

      BindMesh(mesh, opengl_mesh);
    }

    opengl_model.materials.resize(model.materials.size());
    for (size_t i = 0; i < model.materials.size(); i++)
    {
      const sandbox_graphics::Material &material = model.materials[i];
      OpenglMaterial &opengl_material = opengl_model.materials[i];

      opengl_material.base_color = material.base_color;
      opengl_material.metallicity = (float)material.metallicity;
      opengl_material.roughness = (float)material.roughness;
      opengl_material.occlusion = (float)material.occlusion;
      opengl_material.emissive = material.emissive;

      opengl_material.alpha_mode = material.alpha_mode;
      opengl_material.alpha_cutoff = (float)material.alpha_cutoff;
      opengl_material.double_sided = material.double_sided;

      opengl_material.textures.resize(material.textures.size());
      for (size_t j = 0; j < material.textures.size(); j++)
      {
        const sandbox_graphics::Texture &texture = material.textures[j];
        OpenglTexture &opengl_texture = opengl_material.textures[j];

        opengl_texture.type = texture.type;
        GLenum target = GetOpenglTextureTarget(texture.target);
        glGenTextures(1, &opengl_texture.id);
        glBindTexture(target, opengl_texture.id);

        GLenum format = GL_RGBA;
        if (texture.image.channels == 3)
          format = GL_RGB;
        if (texture.type == sandbox_graphics::TextureType::NORMAL)
          glTexImage2D(target, 0, GL_RGBA, texture.image.width, texture.image.height, 0, format, GL_UNSIGNED_BYTE, texture.image.data.data());
        else
          glTexImage2D(target, 0, GL_SRGB_ALPHA, texture.image.width, texture.image.height, 0, format, GL_UNSIGNED_BYTE, texture.image.data.data());
        glGenerateMipmap(target);
        
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GetOpenglTextureWrapMode(texture.wrap_s_mode));
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GetOpenglTextureWrapMode(texture.wrap_t_mode));
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GetOpenglTextureMinFilterMode(texture.min_filter_mode));
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GetOpenglTextureMagFilterMode(texture.mag_filter_mode));
      }
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  void OpenglBinder::UnbindModel(OpenglModel& opengl_model)
  {
    for (auto mesh : opengl_model.meshes)
    {
      glDeleteVertexArrays(1, &mesh.vao);
      glDeleteBuffers(1, &mesh.vbo);

      if (mesh.indices_used)
        glDeleteBuffers(1, &mesh.ebo);
    }
  }

  sandbox_utils::OperationResult OpenglBinder::BindHdrSkybox(OpenglHdrSkybox &skybox, std::string hdr_filename)
  {
    auto window = sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow();
    if (window.expired())
    {
      return sandbox_utils::OperationResult::FAILURE;
    }

    // bind cubemesh
    sandbox_graphics::Mesh mesh = sandbox_graphics::BasicMeshes::ReversedCube();
    BindMesh(mesh, skybox.cube_mesh);

    OpenglShader equirectangularToCubemapShader(RESOURCE_DIRECTORY + std::string("resources/shaders/cubemap.vs"), RESOURCE_DIRECTORY + std::string("resources/shaders/equirectangular_to_cubemap.fs"));
    OpenglShader irradianceShader(RESOURCE_DIRECTORY + std::string("resources/shaders/cubemap.vs"), RESOURCE_DIRECTORY + std::string("resources/shaders/irradiance_convolution.fs"));
    OpenglShader prefilterShader(RESOURCE_DIRECTORY + std::string("resources/shaders/cubemap.vs"), RESOURCE_DIRECTORY + std::string("resources/shaders/prefilter.fs"));
    OpenglShader brdfShader(RESOURCE_DIRECTORY + std::string("resources/shaders/brdf.vs"), RESOURCE_DIRECTORY + std::string("resources/shaders/brdf.fs"));

    // load hdr image
    sandbox_utils::FloatImage skybox_image;
    sandbox_file::FileManagerProvider::GetFileManager()->LoadFloatImage(hdr_filename, skybox_image, true);
    
    // bind hdr image
    unsigned int hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, skybox_image.width, skybox_image.height, 0, GL_RGB, GL_FLOAT, skybox_image.data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     
    //bind framebuffer and renderbuffer
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 2048, 2048);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    
    // create cubemap
    glGenTextures(1, &skybox.cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // create camera positions
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    // load equirectangularToCubemap shader
    equirectangularToCubemapShader.Use();
    equirectangularToCubemapShader.SetInt("TEXTURE_EQUIRECTANGULAR", 0);
    equirectangularToCubemapShader.SetMat4("PROJECTION", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 2048, 2048);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
      equirectangularToCubemapShader.SetMat4("VIEW", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox.cubemap, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
      RenderMesh(skybox.cube_mesh);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // irradiance map
    glGenTextures(1, &skybox.irradiance_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.irradiance_map);
    for (unsigned int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    irradianceShader.Use();
    irradianceShader.SetInt("ENVIRONMENT_MAP", 0);
    irradianceShader.SetMat4("PROJECTION", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
      irradianceShader.SetMat4("VIEW", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox.irradiance_map, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      RenderMesh(skybox.cube_mesh);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // prefilter
    glGenTextures(1, &skybox.prefilter_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.prefilter_map);
    for (unsigned int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    prefilterShader.Use();
    prefilterShader.SetInt("ENVIRONMENT_MAP", 0);
    prefilterShader.SetMat4("PROJECTION", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
      unsigned int mipWidth = 128 * std::pow(0.5, mip);
      unsigned int mipHeight = 128 * std::pow(0.5, mip);
      glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
      glViewport(0, 0, mipWidth, mipHeight);
    
      float roughness = (float)mip / (float)(maxMipLevels - 1);
      prefilterShader.SetFloat("ROUGHNESS", roughness);
      for (unsigned int i = 0; i < 6; ++i)
      {
        prefilterShader.SetMat4("VIEW", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skybox.prefilter_map, mip);
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderMesh(skybox.cube_mesh);
      }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // brdf lut
    glGenTextures(1, &skybox.brdf_lut_map);
    glBindTexture(GL_TEXTURE_2D, skybox.brdf_lut_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, skybox.brdf_lut_map, 0);
    
    glViewport(0, 0, 512, 512);
    brdfShader.Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderQuad();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &hdrTexture);

    // reset viewport size
    auto window_size = window.lock()->GetWindowSize();
    glViewport(0, 0, std::get<0>(window_size), std::get<1>(window_size));

    return sandbox_utils::OperationResult::SUCCESS;
  }

  void OpenglBinder::UnbindHdrSkybox(OpenglHdrSkybox &skybox)
  {
    glDeleteVertexArrays(1, &skybox.cube_mesh.vao);
    glDeleteBuffers(1, &skybox.cube_mesh.vbo);

    if (skybox.cube_mesh.indices_used)
      glDeleteBuffers(1, &skybox.cube_mesh.ebo);
  }

  void OpenglBinder::BindMesh(const sandbox_graphics::Mesh &mesh, OpenglMesh &opengl_mesh)
  {
    opengl_mesh.type = GetOpenglMeshType(mesh.type);
    opengl_mesh.material = mesh.material;
    opengl_mesh.vertices_count = mesh.vertices.size();

    glGenVertexArrays(1, &opengl_mesh.vao);
    glGenBuffers(1, &opengl_mesh.vbo);
    if (mesh.indices_used)
      glGenBuffers(1, &opengl_mesh.ebo);

    glBindVertexArray(opengl_mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, opengl_mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, opengl_mesh.vertices_count * sizeof(sandbox_graphics::Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    if (mesh.indices_used)
    {
      opengl_mesh.indices_used = true;
      opengl_mesh.indeces_count = mesh.indices.size();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, opengl_mesh.ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, opengl_mesh.indeces_count * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);
    }
    else
    {
      opengl_mesh.indices_used = false;
      opengl_mesh.indeces_count = -1;
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sandbox_graphics::Vertex), (void*)offsetof(sandbox_graphics::Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sandbox_graphics::Vertex), (void*)offsetof(sandbox_graphics::Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(sandbox_graphics::Vertex), (void*)offsetof(sandbox_graphics::Vertex, tangent));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(sandbox_graphics::Vertex), (void*)offsetof(sandbox_graphics::Vertex, texture_coords));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(sandbox_graphics::Vertex), (void*)offsetof(sandbox_graphics::Vertex, color));

    glBindVertexArray(0);
  }

  int OpenglBinder::GetOpenglMeshType(sandbox_graphics::PrimitiveType type)
  {
    switch (type)
    {
      case sandbox_graphics::PrimitiveType::POINT:
        return GL_POINTS;
      case sandbox_graphics::PrimitiveType::LINE:
        return GL_LINES;
      case sandbox_graphics::PrimitiveType::LINE_LOOP:
        return GL_LINE_LOOP;
      case sandbox_graphics::PrimitiveType::LINE_STRIP:
        return GL_LINE_STRIP;
      case sandbox_graphics::PrimitiveType::TRIANGLE:
        return GL_TRIANGLES;
      case sandbox_graphics::PrimitiveType::TRIANGLE_STRIP:
        return GL_TRIANGLE_STRIP;
      case sandbox_graphics::PrimitiveType::TRIANGLE_FAN:
        return GL_TRIANGLE_FAN;
      default:
        return GL_TRIANGLES;
    }
  }

  int OpenglBinder::GetOpenglTextureTarget(sandbox_graphics::TextureTarget target)
  {
    switch (target)
    {
      case sandbox_graphics::TextureTarget::_1D:
        return GL_TEXTURE_1D;
      case sandbox_graphics::TextureTarget::_2D:
        return GL_TEXTURE_2D;
      case sandbox_graphics::TextureTarget::_3D:
        return GL_TEXTURE_CUBE_MAP;
      default:
        return GL_TEXTURE_2D;
    }
  }

  int OpenglBinder::GetOpenglTextureMinFilterMode(sandbox_graphics::MinFilterMode mode)
  {
    switch (mode)
    {
      case sandbox_graphics::MinFilterMode::NEAREST:
        return GL_NEAREST;
      case sandbox_graphics::MinFilterMode::LINEAR:
        return GL_LINEAR;
      case sandbox_graphics::MinFilterMode::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
      case sandbox_graphics::MinFilterMode::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
      case sandbox_graphics::MinFilterMode::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
      case sandbox_graphics::MinFilterMode::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
      default:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
  }

  int OpenglBinder::GetOpenglTextureMagFilterMode(sandbox_graphics::MagFilterMode mode)
  {
    switch (mode)
    {
      case sandbox_graphics::MinFilterMode::NEAREST:
        return GL_NEAREST;
      case sandbox_graphics::MinFilterMode::LINEAR:
        return GL_LINEAR;
      default:
        return GL_LINEAR;
    }
  }

  int OpenglBinder::GetOpenglTextureWrapMode(sandbox_graphics::WrapMode mode)
  {
    switch (mode)
    {
      case sandbox_graphics::WrapMode::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
      case sandbox_graphics::WrapMode::CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
      case sandbox_graphics::WrapMode::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
      case sandbox_graphics::WrapMode::REPEAT:
        return GL_REPEAT;
      default:
        return GL_LINEAR;
    }
  }

  void OpenglBinder::RenderMesh(const OpenglMesh &mesh)
  {
    glBindVertexArray(mesh.vao);
    glDrawElements(mesh.type, mesh.indeces_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  void OpenglBinder::RenderQuad()
  {
    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;
    if (quadVAO == 0)
    {
      float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      };
      glGenVertexArrays(1, &quadVAO);
      glGenBuffers(1, &quadVBO);
      glBindVertexArray(quadVAO);
      glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
}
