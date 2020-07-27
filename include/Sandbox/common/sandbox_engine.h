#pragma once

#include "Sandbox/common/i_module.h"

namespace sandbox_common {
  class SandboxEngine : public IModule
  {
   public:
    SandboxEngine() : IModule() {}
    virtual ~SandboxEngine() = default;

    virtual void Construct() override;
    virtual void Destruct() override;
    virtual void Tick(float delta_time) override;

    int Run();
    void Stop();

   private:
    bool is_running_;
  };
}
