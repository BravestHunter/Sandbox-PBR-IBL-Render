#pragma once

#include <memory>

#include "Sandbox/platform/i_platform_window.h"
#include "Sandbox/platform/i_platform_input.h"

namespace sandbox_platform 
{
  class IPlatform
  {
   public:
    IPlatform() = default;
    virtual ~IPlatform() = default;

    std::weak_ptr<IPlatformWindow> GetPlatformWindow() const;
    std::weak_ptr<IPlatformInput> GetPlatformInput() const;

    virtual double GetTime() const = 0;

   protected:
     std::shared_ptr<IPlatformWindow> window_;
     std::shared_ptr<IPlatformInput> input_;
  };
}
