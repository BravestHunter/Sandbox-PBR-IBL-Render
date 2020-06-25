#pragma once

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Sandbox/platform/i_platform_window.h"

namespace sandbox_platform 
{
  class GlfwWindow : public IPlatformWindow
  {
   public:
    GlfwWindow();
    virtual ~GlfwWindow();

    virtual void Init(int width, int height, std::string title) override;
    virtual void MakeCurrent() override;
    virtual void SwapBuffers() override;
    virtual void CloseWindow() override;

    virtual std::tuple<unsigned int, unsigned int> GetWindowSize() const override;

    GLFWwindow* GetGlfwWindow() const;

   private:
    GLFWwindow* glfw_window_;

    GlfwWindow(const GlfwWindow&) = delete;
    GlfwWindow(GlfwWindow* const) = delete;
  };
}
