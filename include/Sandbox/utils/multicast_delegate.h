#pragma once

#include <map>
#include <set>

#include "Sandbox/utils/delegate.h"
#include "Sandbox/utils/operation_result.h"

namespace sandbox_utils 
{
  template<typename ... ArgTypes>
  class MulticastDelegate
  {
   using DelegateType = Delegate<void, ArgTypes ...>;

   public:
    MulticastDelegate(short int size = 8)
    {
      for (short int i = 0; i < size; i++)
        free_positions_.insert(i);
    }
    ~MulticastDelegate() = default;

    OperationResult Bind(const DelegateType &del, short int &id);
    void InvokeAll(ArgTypes ... args);
    OperationResult Remove(short int delegate_id);

   private:
    std::set<short int> free_positions_;
    std::map<short int, DelegateType> delegates_;
  };

  template<typename ... ArgTypes>
  OperationResult MulticastDelegate<ArgTypes ...>::Bind(const DelegateType &del, short int &id)
  {
    if (free_positions_.size() == 0)
    {
      return OperationResult::FAILURE;
    }

    id = *free_positions_.begin();
    free_positions_.erase(id);
    delegates_.insert(std::pair<short int, DelegateType>(id, del));

    return OperationResult::SUCCESS;
  }

  template<typename ... ArgTypes>
  void MulticastDelegate<ArgTypes ...>::InvokeAll(ArgTypes ... args)
  {
    for (auto &del : delegates_)
    {
      if (del.second.IsBound())
        del.second.Invoke(args ...);
    }
  }

  template<typename ... ArgTypes>
  OperationResult MulticastDelegate<ArgTypes ...>::Remove(short int id)
  {
    if (delegates_.find(id) == delegates_.end())
    {
      return OperationResult::FAILURE;
    }

    free_positions_.insert(id);
    delegates_.erase(id);
  }
}
