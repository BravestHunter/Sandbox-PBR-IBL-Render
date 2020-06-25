#include "Sandbox/platform/i_platform.h"

#include <algorithm>

namespace sandbox_platform
{
  std::weak_ptr<IPlatformWindow> IPlatform::GetPlatformWindow() const
  {
    return window_;
  }

  std::weak_ptr<IPlatformInput> IPlatform::GetPlatformInput() const
  {
    return input_;
  }
}
