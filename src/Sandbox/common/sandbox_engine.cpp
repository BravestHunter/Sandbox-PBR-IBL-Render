#include "Sandbox/common/sandbox_engine.h"

#include "Sandbox/platform/platform_provider.h"
#include "Sandbox/log/logger_provider.h"

namespace sandbox_common {
  void SandboxEngine::Construct()
  {
    is_constructed_ = true;
  }

  void SandboxEngine::Destruct()
  {
    is_constructed_ = false;
  }

  void SandboxEngine::Tick(float delta_time)
  {
    if (!is_constructed_)
      return;

    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->UpdateInput();
  }

  int SandboxEngine::Run()
  {
    if (!is_constructed_)
    {
      return -1;
    }

    is_running_ = true;
    auto platform = sandbox_platform::PlatformProvider::GetPlatform();
    float old_time = platform->GetTime();
    while (is_running_)
    {
      const float new_time = platform->GetTime();
      const float delta_time = new_time - old_time;
      old_time = new_time;

      TickAll(delta_time);
    }

    return 0;
  }

  void SandboxEngine::Stop()
  {
    is_running_ = false;
  }
}
