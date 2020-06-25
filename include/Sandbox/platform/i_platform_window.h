#pragma once

#include <string>
#include <tuple>

namespace sandbox_platform
{
  class IPlatformWindow
  {
   public:
    IPlatformWindow() = default;
    virtual ~IPlatformWindow() = default;

    virtual void Init(int width, int height, std::string title) = 0;
    virtual void MakeCurrent() = 0;
    virtual void SwapBuffers() = 0;
    virtual void CloseWindow() = 0;

    virtual std::tuple<unsigned int, unsigned int> GetWindowSize() const = 0;
  };
}
