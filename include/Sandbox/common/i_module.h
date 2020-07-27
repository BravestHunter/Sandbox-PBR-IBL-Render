#pragma once

#include <memory>
#include <vector>

#include "Sandbox/utils/multicast_delegate.h"
#include "Sandbox/utils/input.h"

namespace sandbox_common {
  class IModule : public std::enable_shared_from_this<IModule>
  {
   public:
    virtual void Construct();
    virtual void Destruct();
    virtual void Tick(float delta_time) = 0;

    void TickAll(float delta_time);
    void AddChild(std::shared_ptr<IModule> child);

    bool IsConstructed() const;

   protected:
    bool is_constructed_ = false;
    bool traversal_order_;
    std::weak_ptr<IModule> parent_;
    std::vector<std::shared_ptr<IModule>> children_;

    IModule(bool traversal_order = false) : traversal_order_(traversal_order) {}
    virtual ~IModule() = default;

   private:
    void SetParent(std::weak_ptr<IModule> parent);
  };
}
