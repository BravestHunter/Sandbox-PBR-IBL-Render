#include "Sandbox/graphics/model_loader.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Sandbox/utils/util_functions.h"

namespace sandbox_graphics
{
  // gltf functions
  std::vector<glm::vec2> GetGltfVec2DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor);
  std::vector<glm::vec3> GetGltfVec3DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor);
  std::vector<glm::vec4> GetGltfVec4DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor);
  std::vector<unsigned int> GetGltfUIntDataVector(const tinygltf::Model &model, tinygltf::Accessor accessor);
  void GetGltfTexture(Texture& texture, const tinygltf::Image& gltf_image, const tinygltf::Sampler& gltf_sampler);

  sandbox_utils::OperationResult ModelLoader::LoadGltfModel(GraphicsModel &model, const std::string filename) // todo: don't use third party libs for model loading
  {
    model.type = ModelType::GLTF;

    tinygltf::Model gltf_model;
    tinygltf::TinyGLTF gltf_loader;
    std::string error;
    std::string warning;

    gltf_loader.SetStoreOriginalJSONForExtrasAndExtensions(false);

    bool result;
    if (sandbox_utils::GetFileExtension(filename).compare("glb") == 0)
    {
      result = gltf_loader.LoadBinaryFromFile(&gltf_model, &error, &warning, filename);
    }
    else
    {
      result = gltf_loader.LoadASCIIFromFile(&gltf_model, &error, &warning, filename);
    }

    // log warnings
    // log errors
    if (!result)
    {
      // log error
    }

    // loading meshes
    for (size_t i = 0; i < gltf_model.nodes.size(); i++)
    {
      const tinygltf::Node &gltf_node = gltf_model.nodes[i];

      bool use_transform = gltf_node.matrix.size() > 0 || gltf_node.translation.size() > 0 || gltf_node.rotation.size() > 0 || gltf_node.scale.size() > 0;
      glm::mat4 transform = glm::mat4(1.0f);
      if (use_transform)
      {
        if (gltf_node.matrix.size() > 0)
        {
          float f_mat[16];
          for (int i = 0; i < 16; i++)
            f_mat[i] = (float)gltf_node.matrix[i];
          transform = glm::make_mat4(f_mat);
        }
        else
        {
          glm::mat4 translate = glm::mat4(1.0f);
          glm::mat4 rotate = glm::mat4(1.0f);
          glm::mat4 scale = glm::mat4(1.0f);

          if (gltf_node.translation.size() > 0)
            translate = glm::translate(glm::mat4(1.0), glm::vec3((float)gltf_node.translation[0], (float)gltf_node.translation[1], (float)gltf_node.translation[2]));

          if (gltf_node.rotation.size() > 0)
            rotate = glm::toMat4(glm::quat((float)gltf_node.rotation[0], (float)gltf_node.rotation[1], (float)gltf_node.rotation[2], (float)gltf_node.rotation[3]));

          if (gltf_node.scale.size() > 0)
            scale = glm::scale(glm::mat4(1.0), glm::vec3((float)gltf_node.scale[0], (float)gltf_node.scale[1], (float)gltf_node.scale[2]));

          transform = translate * rotate * scale;
        }
      }

      if (gltf_node.mesh > -1)
      {
        const tinygltf::Mesh &gltf_mesh = gltf_model.meshes[gltf_node.mesh];
        for (size_t j = 0; j < gltf_mesh.primitives.size(); j++)
        {
          const tinygltf::Primitive gltf_primitive = gltf_mesh.primitives[j];
          Mesh mesh;

          mesh.type = GetGltfPrimitiveType(gltf_primitive.mode);
          mesh.material = gltf_primitive.material;

          tinygltf::Accessor position_accessor = gltf_model.accessors[(*gltf_primitive.attributes.find("POSITION")).second];
          std::vector<glm::vec3> position_data = GetGltfVec3DataVector(gltf_model, position_accessor);

          bool has_normals = gltf_primitive.attributes.find("NORMAL") != gltf_primitive.attributes.end();
          bool has_tangents = gltf_primitive.attributes.find("TANGENT") != gltf_primitive.attributes.end();
          bool has_texcoords = gltf_primitive.attributes.find("TEXCOORD_0") != gltf_primitive.attributes.end();
          bool has_colors = gltf_primitive.attributes.find("COLOR_0") != gltf_primitive.attributes.end();
          std::vector<glm::vec3> normal_data;
          std::vector<glm::vec4> tangent_data;
          std::vector<glm::vec2> texcoords_data;
          std::vector<glm::vec4> colors_data;
          if (has_normals)
            normal_data = GetGltfVec3DataVector(gltf_model, gltf_model.accessors[(*gltf_primitive.attributes.find("NORMAL")).second]);
          if (has_tangents)
            tangent_data = GetGltfVec4DataVector(gltf_model, gltf_model.accessors[(*gltf_primitive.attributes.find("TANGENT")).second]);
          if (has_texcoords)
            texcoords_data = GetGltfVec2DataVector(gltf_model, gltf_model.accessors[(*gltf_primitive.attributes.find("TEXCOORD_0")).second]);
          if (has_colors)
            colors_data = GetGltfVec4DataVector(gltf_model, gltf_model.accessors[(*gltf_primitive.attributes.find("COLOR_0")).second]);

          mesh.vertices.resize(position_accessor.count);
          for (size_t k = 0; k < position_accessor.count; k++)
          {
             mesh.vertices[k].position = use_transform ? glm::vec3(transform * glm::vec4(position_data[k], 1.0)) : position_data[k];

            if (has_normals)
              mesh.vertices[k].normal = use_transform ? glm::vec3(transform * glm::vec4(normal_data[k], 1.0)) : normal_data[k];
            if (has_tangents)
              mesh.vertices[k].tangent = use_transform ? transform * tangent_data[k] : tangent_data[k];
            if (has_texcoords)
              mesh.vertices[k].texture_coords = texcoords_data[k];
            if (has_colors)
              mesh.vertices[k].color = colors_data[k];
          }

          if (gltf_primitive.indices > -1)
          {
            mesh.indices = GetGltfUIntDataVector(gltf_model, gltf_model.accessors[gltf_primitive.indices]);
            mesh.indices_used = true;
          }
          else
          {
            mesh.indices_used = false;
          }

          if (!has_tangents)
            GenerateTangents(mesh);

          model.meshes.push_back(mesh);
        }
      }
    }

    // loading materials
    model.materials.resize(gltf_model.materials.size());
    for (size_t i = 0; i < gltf_model.materials.size(); i++)
    {
      const tinygltf::Material &gltf_material = gltf_model.materials[i];
      Material &material = model.materials[i];
      material.name = gltf_material.name;
      material.alpha_mode = GetGltfAlphaMode(gltf_material.alphaMode);
      material.alpha_cutoff = gltf_material.alphaCutoff;
      material.double_sided = gltf_material.doubleSided;

      // base color
      material.base_color = glm::vec4(
        gltf_material.pbrMetallicRoughness.baseColorFactor[0], 
        gltf_material.pbrMetallicRoughness.baseColorFactor[1],
        gltf_material.pbrMetallicRoughness.baseColorFactor[2],
        gltf_material.pbrMetallicRoughness.baseColorFactor[3]);
      if (gltf_material.pbrMetallicRoughness.baseColorTexture.index > -1)
      {
        const tinygltf::Texture &gltf_texture = gltf_model.textures[gltf_material.pbrMetallicRoughness.baseColorTexture.index];
        const tinygltf::Image &gltf_image = gltf_model.images[gltf_texture.source];
        const tinygltf::Sampler &gltf_sampler = gltf_texture.sampler > -1 ? gltf_model.samplers[gltf_texture.sampler] : tinygltf::Sampler();

        Texture texture;
        texture.type = sandbox_graphics::TextureType::BASE_COLOR;
        texture.target = sandbox_graphics::TextureTarget::_2D;
        GetGltfTexture(texture, gltf_image, gltf_sampler);

        material.textures.push_back(texture);
      }

      // metallic-roughness
      material.metallicity = gltf_material.pbrMetallicRoughness.metallicFactor;
      material.roughness = gltf_material.pbrMetallicRoughness.roughnessFactor;
      if (gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index > -1)
      {
        const tinygltf::Texture &gltf_texture = gltf_model.textures[gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index];
        const tinygltf::Image &gltf_image = gltf_model.images[gltf_texture.source];
        const tinygltf::Sampler &gltf_sampler = gltf_texture.sampler > -1 ? gltf_model.samplers[gltf_texture.sampler] : tinygltf::Sampler();

        Texture texture;
        texture.type = sandbox_graphics::TextureType::METALLIC_ROUGHNESS;
        texture.target = sandbox_graphics::TextureTarget::_2D;
        GetGltfTexture(texture, gltf_image, gltf_sampler);

        material.textures.push_back(texture);
      }

      // normal
      if (gltf_material.normalTexture.index > -1)
      {
        const tinygltf::Texture &gltf_texture = gltf_model.textures[gltf_material.normalTexture.index];
        const tinygltf::Image &gltf_image = gltf_model.images[gltf_texture.source];
        const tinygltf::Sampler &gltf_sampler = gltf_texture.sampler > -1 ? gltf_model.samplers[gltf_texture.sampler] : tinygltf::Sampler();

        Texture texture;
        texture.type = sandbox_graphics::TextureType::NORMAL;
        texture.target = sandbox_graphics::TextureTarget::_2D;
        GetGltfTexture(texture, gltf_image, gltf_sampler);

        material.textures.push_back(texture);
      }

      // occlusion
      material.occlusion = 1.0;
      if (gltf_material.occlusionTexture.index > -1)
      {
        const tinygltf::Texture &gltf_texture = gltf_model.textures[gltf_material.occlusionTexture.index];
        const tinygltf::Image &gltf_image = gltf_model.images[gltf_texture.source];
        const tinygltf::Sampler &gltf_sampler = gltf_texture.sampler > -1 ? gltf_model.samplers[gltf_texture.sampler] : tinygltf::Sampler();

        Texture texture;
        texture.type = sandbox_graphics::TextureType::OCCLUSION;
        texture.target = sandbox_graphics::TextureTarget::_2D;
        GetGltfTexture(texture, gltf_image, gltf_sampler);

        material.textures.push_back(texture);
      }

      // emissive
      if (gltf_material.emissiveTexture.index > -1)
      {
        const tinygltf::Texture &gltf_texture = gltf_model.textures[gltf_material.emissiveTexture.index];
        const tinygltf::Image &gltf_image = gltf_model.images[gltf_texture.source];
        const tinygltf::Sampler &gltf_sampler = gltf_texture.sampler > -1 ? gltf_model.samplers[gltf_texture.sampler] : tinygltf::Sampler();

        Texture texture;
        texture.type = sandbox_graphics::TextureType::EMISSIVE;
        texture.target = sandbox_graphics::TextureTarget::_2D;
        GetGltfTexture(texture, gltf_image, gltf_sampler);

        material.textures.push_back(texture);
      }
    }

    return sandbox_utils::OperationResult::SUCCESS;
  }

  PrimitiveType ModelLoader::GetGltfPrimitiveType(int type)
  {
    switch (type)
    {
      case TINYGLTF_MODE_POINTS:
        return PrimitiveType::POINT;
      case TINYGLTF_MODE_LINE:
        return PrimitiveType::LINE;
      case TINYGLTF_MODE_LINE_LOOP:
        return PrimitiveType::LINE_LOOP;
      case TINYGLTF_MODE_LINE_STRIP:
        return PrimitiveType::LINE_STRIP;
      case TINYGLTF_MODE_TRIANGLES:
        return PrimitiveType::TRIANGLE;
      case TINYGLTF_MODE_TRIANGLE_STRIP:
        return PrimitiveType::TRIANGLE_STRIP;
      case TINYGLTF_MODE_TRIANGLE_FAN:
        return PrimitiveType::TRIANGLE_FAN;
      default:
        return PrimitiveType::UNKNOWN;
    }
  }

  void ModelLoader::GenerateTangents(Mesh &mesh)
  {
    if (mesh.indices_used)
    {
      size_t triangles_count = mesh.indices.size() / 3;
      for (size_t i = 0; i < triangles_count; i++)
      {
        Vertex &vertex1 = mesh.vertices[mesh.indices[i * 3]];
        Vertex &vertex2 = mesh.vertices[mesh.indices[i * 3 + 1]];
        Vertex &vertex3 = mesh.vertices[mesh.indices[i * 3 + 2]];

        ModelLoader::GenerateTangent(vertex1, vertex2, vertex3);
      }
    }
    else
    {
      size_t triangles_count = mesh.vertices.size() / 3;
      for (size_t i = 0; i < triangles_count; i++)
      {
        Vertex &vertex1 = mesh.vertices[i * 3 + 0];
        Vertex &vertex2 = mesh.vertices[i * 3 + 1];
        Vertex &vertex3 = mesh.vertices[i * 3 + 2];

        ModelLoader::GenerateTangent(vertex1, vertex2, vertex3);
      }
    }
  }

  void ModelLoader::GenerateTangent(Vertex &vertex1, Vertex &vertex2, Vertex &vertex3)
  {
    glm::vec3 edge1 = vertex2.position - vertex1.position;
    glm::vec3 edge2 = vertex3.position - vertex1.position;
    glm::vec2 deltaUV1 = vertex2.texture_coords - vertex1.texture_coords;
    glm::vec2 deltaUV2 = vertex3.texture_coords - vertex1.texture_coords;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    vertex1.tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    vertex1.tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    vertex1.tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    vertex1.tangent = glm::normalize(vertex1.tangent);
    vertex2.tangent = vertex1.tangent;
    vertex3.tangent = vertex1.tangent;
  }

  MinFilterMode ModelLoader::GetGltfMinFilterMode(int mode)
  {
    switch (mode)
    {
      case TINYGLTF_TEXTURE_FILTER_NEAREST:
        return MinFilterMode::NEAREST;
      case TINYGLTF_TEXTURE_FILTER_LINEAR:
        return MinFilterMode::LINEAR;
      case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
        return MinFilterMode::NEAREST_MIPMAP_LINEAR;
      case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
        return MinFilterMode::LINEAR_MIPMAP_NEAREST;
      case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
        return MinFilterMode::NEAREST_MIPMAP_LINEAR;
      case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
        return MinFilterMode::LINEAR_MIPMAP_LINEAR;
      default:
        return MinFilterMode::UNKNOWN;
    }
  }

  MagFilterMode ModelLoader::GetGltfMagFilterMode(int mode)
  {
    switch (mode)
    {
      case TINYGLTF_TEXTURE_FILTER_NEAREST:
        return MagFilterMode::NEAREST;
      case TINYGLTF_TEXTURE_FILTER_LINEAR:
        return MagFilterMode::LINEAR;
      default:
        return MagFilterMode::UNKNOWN;
    }
  }

  WrapMode ModelLoader::GetGltfWrapMode(int mode)
  {
    switch (mode)
    {
      case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
        return WrapMode::CLAMP_TO_EDGE;
      case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
        return WrapMode::MIRRORED_REPEAT;
      case TINYGLTF_TEXTURE_WRAP_REPEAT:
        return WrapMode::REPEAT;
      default:
        return WrapMode::UNKNOWN;
    }
  }

  AlphaMode ModelLoader::GetGltfAlphaMode(const std::string mode)
  {
    if (mode == "OPAQUE") return AlphaMode::OPAQUe;
    if (mode == "BLEND") return AlphaMode::BLEND;
    if (mode == "MASK") return AlphaMode::MASK;
    return AlphaMode::OPAQUe;
  }

  std::vector<glm::vec2> GetGltfVec2DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[buffer_view.buffer];

    int size = tinygltf::GetComponentSizeInBytes(accessor.componentType) * 2;
    std::vector<glm::vec2> result(accessor.count);

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
    {
      for (size_t i = 0; i < accessor.count; i++)
      {
        static double d[2];
        if (buffer_view.byteStride > 0)
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, 16);
        else
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * 16, 16);

        result[i][0] = (float)d[0];
        result[i][1] = (float)d[1];
      }
    }
    else
    {
      if (buffer_view.byteStride > 0)
      {
        for (size_t i = 0; i < accessor.count; i++)
        {
          memcpy(result.data() + i, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, size);
        }
      }
      else
        memcpy(result.data(), buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset, accessor.count * size);
    }

    return result;
  }

  std::vector<glm::vec3> GetGltfVec3DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[buffer_view.buffer];

    int size = tinygltf::GetComponentSizeInBytes(accessor.componentType) * 3;
    std::vector<glm::vec3> result(accessor.count);

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
    {
      for (size_t i = 0; i < accessor.count; i++)
      {
        static double d[3];
        if (buffer_view.byteStride > 0)
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, 24);
        else
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * 24, 24);

        result[i][0] = (float)d[0];
        result[i][1] = (float)d[1];
        result[i][2] = (float)d[2];
      }
    }
    else // checked
    {
      if (buffer_view.byteStride > 0)
      {
        for (size_t i = 0; i < accessor.count; i++)
        {
          memcpy(result.data() + i, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, size);
        }
      }
      else
        memcpy(result.data(), buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset, accessor.count * size);
    }

    return result;
  }

  std::vector<glm::vec4> GetGltfVec4DataVector(const tinygltf::Model &model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[buffer_view.buffer];

    int size = tinygltf::GetComponentSizeInBytes(accessor.componentType) * 4;
    std::vector<glm::vec4> result(accessor.count);

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE)
    {
      for (size_t i = 0; i < accessor.count; i++)
      {
        static double d[4];
        if (buffer_view.byteStride > 0)
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, 32);
        else
          memcpy(&d, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * 32, 32);

        result[i][0] = (float)d[0];
        result[i][1] = (float)d[1];
        result[i][2] = (float)d[2];
        result[i][3] = (float)d[3];
      }
    }
    else
    {
      if (buffer_view.byteStride > 0)
      {
        for (size_t i = 0; i < accessor.count; i++)
        {
          memcpy(result.data() + i, buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset + i * buffer_view.byteStride, size);
        }
      }
      else
        memcpy(result.data(), buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset, accessor.count * size);
    }

    return result;
  }

  std::vector<unsigned int> GetGltfUIntDataVector(const tinygltf::Model &model, tinygltf::Accessor accessor)
  {
    const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer buffer = model.buffers[buffer_view.buffer];

    std::vector<unsigned int> result(accessor.count);

    switch (accessor.componentType)
    {
    case TINYGLTF_COMPONENT_TYPE_BYTE:
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      for (size_t i = 0; i < accessor.count; i++)
      {
        result[i] = (unsigned int)buffer.data[i + buffer_view.byteOffset + accessor.byteOffset];
      }
      break;
    case TINYGLTF_COMPONENT_TYPE_SHORT:
      for (size_t i = 0; i < accessor.count; i++)
      {
        static short s;

        memcpy(&s, &buffer.data.at(buffer_view.byteOffset + accessor.byteOffset + i * 2), 2);

        result[i] = (unsigned int)s;
      }
      break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      for (size_t i = 0; i < accessor.count; i++)
      {
        static unsigned short us;

        memcpy(&us, &buffer.data.at(0) + buffer_view.byteOffset + accessor.byteOffset + i * 2, 2);

        result[i] = (unsigned int)us;
      }
      break;
    default:
      memcpy(result.data(), buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset, accessor.count * tinygltf::GetComponentSizeInBytes(accessor.componentType));
      break;
    }

    return result;
  }

  void GetGltfTexture(Texture& texture, const tinygltf::Image& gltf_image, const tinygltf::Sampler& gltf_sampler)
  {
    texture.min_filter_mode = ModelLoader::GetGltfMinFilterMode(gltf_sampler.minFilter);
    texture.mag_filter_mode = ModelLoader::GetGltfMagFilterMode(gltf_sampler.magFilter);
    texture.wrap_s_mode = ModelLoader::GetGltfWrapMode(gltf_sampler.wrapS);
    texture.wrap_t_mode = ModelLoader::GetGltfWrapMode(gltf_sampler.wrapT);

    sandbox_utils::Image &image = texture.image;
    image.data = std::vector<unsigned char>(gltf_image.image);
    image.width = gltf_image.width;
    image.height = gltf_image.height;
    image.channels = gltf_image.component;
    image.channel_bits = gltf_image.bits;
  }
}
