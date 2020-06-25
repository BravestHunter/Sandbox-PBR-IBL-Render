#include "Sandbox/platform/platform_provider.h"

#include "Sandbox/platform/glfw/glfw_platform.h"

namespace sandbox_platform
{
  std::shared_ptr<IPlatform> PlatformProvider::platform_;
  bool PlatformProvider::is_set_(false);

  sandbox_utils::OperationResult PlatformProvider::SetPlatform(std::shared_ptr<IPlatform> platform)
  {
    if (!is_set_)
    {
      PlatformProvider::platform_ = platform;
      is_set_ = true;
      return sandbox_utils::OperationResult::SUCCESS;
    }

    return sandbox_utils::OperationResult::FAILURE;
  }

  std::shared_ptr<IPlatform> PlatformProvider::GetPlatform()
  {
    if (!is_set_)
    {
      SetPlatform(std::static_pointer_cast<IPlatform>(std::make_shared<GlfwPlatform>()));
    }

    return PlatformProvider::platform_;
  }
}
