#pragma once

#include <tuple>

#include "Sandbox/platform/i_platform.h"

namespace sandbox_platform 
{
  class GlfwPlatform : public IPlatform
  {
   public:
    GlfwPlatform();
    ~GlfwPlatform();

    std::tuple<int, int> GetContextVersion() const;
    void SetContextVersion(int major, int minor);

    virtual double GetTime() const override;

  private:
    int context_version_major_ = 3;
    int context_version_minor_ = 3;
  };
}
