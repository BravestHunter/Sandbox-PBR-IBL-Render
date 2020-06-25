#pragma once

#include <memory>

#include "Sandbox/platform/i_platform.h"
#include "Sandbox/utils/operation_result.h"

namespace sandbox_platform
{
  class PlatformProvider
  {
   public:
    static sandbox_utils::OperationResult SetPlatform(std::shared_ptr<IPlatform> platform);
    static std::shared_ptr<IPlatform> GetPlatform();

   private:
    static std::shared_ptr<IPlatform> platform_;
    static bool is_set_;
  };
}
