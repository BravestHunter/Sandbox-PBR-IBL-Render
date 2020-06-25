#include "Sandbox/platform/glfw/glfw_window.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace sandbox_platform 
{
  GlfwWindow::GlfwWindow()
  {
    // constructor
  }

  GlfwWindow::~GlfwWindow()
  {
    CloseWindow();
  }

  void GlfwWindow::Init(int width, int height, std::string title)
  {
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfw_window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!glfw_window_)
    {
      // log glfw window creation error
    }

    glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // vsync
    glfwSwapInterval(1);

    glViewport(0, 0, width, height);
  }

  void GlfwWindow::MakeCurrent()
  {
    glfwMakeContextCurrent(glfw_window_);
  }

  void GlfwWindow::SwapBuffers()
  {
    glfwSwapBuffers(glfw_window_);
  }

  void GlfwWindow::CloseWindow()
  {
    glfwDestroyWindow(glfw_window_);
    glfw_window_ = nullptr;
  }

  std::tuple<unsigned int, unsigned int> GlfwWindow::GetWindowSize() const
  {
    int width, height;
    glfwGetWindowSize(glfw_window_, &width, &height);
    return { width, height };
  }

  GLFWwindow* GlfwWindow::GetGlfwWindow() const
  {
    return glfw_window_;
  }
}
