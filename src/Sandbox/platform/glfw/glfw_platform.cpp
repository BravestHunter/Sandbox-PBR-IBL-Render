#include "Sandbox/platform/glfw/glfw_platform.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Sandbox/platform/glfw/glfw_window.h"
#include "Sandbox/platform/glfw/glfw_input.h"

namespace sandbox_platform 
{
  GlfwPlatform::GlfwPlatform()
  {
    if (!glfwInit())
    {
      // log glfw init error
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor_);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = std::make_shared<GlfwWindow>();
    input_ = std::make_shared<GlfwInput>();
  }

  GlfwPlatform::~GlfwPlatform()
  {
    glfwTerminate();
  }

  std::tuple<int, int> GlfwPlatform::GetContextVersion() const
  {
    return { context_version_major_ , context_version_minor_ };
  }

  void GlfwPlatform::SetContextVersion(int major, int minor)
  {
    context_version_major_ = major;
    context_version_minor_ = minor;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_version_major_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_version_minor_);
  }

  double GlfwPlatform::GetTime() const
  {
    return glfwGetTime();
  }
}
