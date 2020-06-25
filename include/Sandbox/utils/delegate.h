#pragma once

#include <functional>
#include <memory>

namespace sandbox_utils 
{
  template<typename ReturnType, typename ... ArgTypes>
  class Delegate
  {
    using DelegateType = Delegate<ReturnType, ArgTypes ...>;

   public:
    ~Delegate() = default;

    Delegate(const Delegate&) = default;
    Delegate(Delegate&&) = default;

    static DelegateType CreateDelegate(const std::function<ReturnType(ArgTypes ...)> &func);
    static DelegateType CreateWeakDelegate(const std::weak_ptr<void>& object_ptr, const std::function<ReturnType(ArgTypes ...)> &func);

    bool IsBound() const;
    ReturnType Invoke(ArgTypes ... args);

   private:
    Delegate()
      : is_weak_(false),
      object_ptr_(nullptr) {}
    Delegate(const std::function<ReturnType(ArgTypes ...)> &func)
      : is_weak_(false),
      object_ptr_(std::weak_ptr<void>()),
      func_(func) {}
    Delegate(const std::weak_ptr<void>& object_ptr, const std::function<ReturnType(ArgTypes ...)> &func)
      : is_weak_(true),
      object_ptr_(object_ptr),
      func_(func) {}

    bool is_weak_;
    std::function<ReturnType(ArgTypes ...)> func_;
    std::weak_ptr<void> object_ptr_;
  };

  template<typename ReturnType, typename ... ArgTypes>
  Delegate<ReturnType, ArgTypes ...> Delegate<ReturnType, ArgTypes ...>::CreateDelegate(const std::function<ReturnType(ArgTypes ...)> &func)
  {
    return Delegate<ReturnType, ArgTypes ...>(func);
  }

  template<typename ReturnType, typename ... ArgTypes>
  Delegate<ReturnType, ArgTypes ...> Delegate<ReturnType, ArgTypes ...>::CreateWeakDelegate(const std::weak_ptr<void>& object_ptr, const std::function<ReturnType(ArgTypes ...)> &func)
  {
    return Delegate<ReturnType, ArgTypes ...>(object_ptr, func);
  }

  template<typename ReturnType, typename ... ArgTypes>
  bool Delegate<ReturnType, ArgTypes ...>::IsBound() const
  {
    if (is_weak_)
    {
      return !object_ptr_.expired() && (func_ ? true : false);
    }

    return func_ ? true : false;
  }

  template<typename ReturnType, typename ... ArgTypes>
  ReturnType Delegate<ReturnType, ArgTypes ...>::Invoke(ArgTypes ... args)
  {
    return func_(args ...);
  }
}