#include "Sandbox/common/i_module.h"

namespace sandbox_common {
  void IModule::Construct()
  {
    is_constructed_ = true;
  }

  void IModule::Destruct()
  {
    is_constructed_ = false;
  }

  void IModule::TickAll(float delta_time)
  {
    if (traversal_order_)
      Tick(delta_time);

    for (const auto child : children_)
    {
      child->TickAll(delta_time);
    }

    if (!traversal_order_)
      Tick(delta_time);
  }

  void IModule::AddChild(std::shared_ptr<IModule> child)
  {
    children_.push_back(child);
    child->SetParent(shared_from_this());
  }

  bool IModule::IsConstructed() const
  {
    return is_constructed_;
  }

  void IModule::SetParent(std::weak_ptr<IModule> parent)
  {
    parent_ = parent;
  }
}
