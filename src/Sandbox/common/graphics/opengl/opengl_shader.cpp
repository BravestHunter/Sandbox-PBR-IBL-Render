#include <Sandbox/common/graphics/opengl/opengl_shader.h>

#include <GL/glew.h>

#include "Sandbox/file/file_manager_provider.h"

namespace sandbox_common {
  OpenglShader::OpenglShader(const std::string vertex_shader_path, const std::string fragment_shader_path, const std::string geometry_shader_path)
  {
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    std::string geometry_shader_code;

    sandbox_file::FileManagerProvider::GetFileManager()->LoadFile(vertex_shader_path, vertex_shader_code);
    sandbox_file::FileManagerProvider::GetFileManager()->LoadFile(fragment_shader_path, fragment_shader_code);
    if (geometry_shader_path != "")
    {
      sandbox_file::FileManagerProvider::GetFileManager()->LoadFile(geometry_shader_path, geometry_shader_code);
    }

    const char* vertex_shader_code_c = vertex_shader_code.c_str();
    const char * fragment_shader_code_c = fragment_shader_code.c_str();

    unsigned int vertex, fragment, geometry;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code_c, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code_c, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    if (geometry_shader_path != "")
    {
      const char * geometry_shader_code_c = geometry_shader_code.c_str();
      geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry, 1, &geometry_shader_code_c, NULL);
      glCompileShader(geometry);
      CheckCompileErrors(geometry, "GEOMETRY");
    }

    id_ = glCreateProgram();
    glAttachShader(id_, vertex);
    glAttachShader(id_, fragment);
    if (geometry_shader_path != "")
      glAttachShader(id_, geometry);
    glLinkProgram(id_);
    CheckCompileErrors(id_, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry_shader_path != "")
      glDeleteShader(geometry);
  }

  void OpenglShader::Use() const
  {
    glUseProgram(id_);
  }

  unsigned int OpenglShader::GetId() const
  {
    return id_;
  }

  void OpenglShader::SetBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
  }

  void OpenglShader::SetInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }

  void OpenglShader::SetFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }

  void OpenglShader::SetVec2(const std::string &name, const glm::vec2 &value) const
  {
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void OpenglShader::SetVec2(const std::string &name, float x, float y) const
  {
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
  }

  void OpenglShader::SetVec3(const std::string &name, const glm::vec3 &value) const
  {
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void OpenglShader::SetVec3(const std::string &name, float x, float y, float z) const
  {
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
  }

  void OpenglShader::SetVec4(const std::string &name, const glm::vec4 &value) const
  {
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void OpenglShader::SetVec4(const std::string &name, float x, float y, float z, float w) const
  {
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
  }

  void OpenglShader::SetMat2(const std::string &name, const glm::mat2 &mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void OpenglShader::SetMat3(const std::string &name, const glm::mat3 &mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void OpenglShader::SetMat4(const std::string &name, const glm::mat4 &mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void OpenglShader::CheckCompileErrors(unsigned int shader, std::string type)
  {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        // log error
      }
    }
    else
    {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        // log error
      }
    }
  }
}
