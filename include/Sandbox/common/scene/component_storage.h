#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>

#include "Sandbox/utils/operation_result.h"
#include "Sandbox/common/scene/scene_base.h"

namespace sandbox_common
{
  class IComponentStorage
  {
   public:
    virtual ~IComponentStorage() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
  };

  template<typename T>
  class ComponentStorage : public IComponentStorage
  {
   public:
    template<typename T>
    struct StorageItem
    {
      bool exist;
      T data;
    };

    sandbox_utils::OperationResult InsertComponent(Entity entity, T component)
    {
      if (entity_to_index_.find(entity) != entity_to_index_.end())
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      if (free_indices_.empty())
      {
        data_vector_.push_back({ true, component });
        entity_to_index_[entity] = data_vector_.size() - 1;
      }
      else
      {
        size_t index = free_indices_.front();
        free_indices_.pop();
        entity_to_index_[entity] = index;
        data_vector_[index].exist = true;
        data_vector_[index].data = component;
      }

      return sandbox_utils::OperationResult::SUCCESS;
    }

    sandbox_utils::OperationResult RemoveComponent(Entity entity)
    {
      if (entity_to_index_.find(entity) == entity_to_index_.end())
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      size_t index = entity_to_index_[entity];
      data_vector_[index].exist = false;
      data_vector_[index].data.~T();
      free_indices_.push(index);
      entity_to_index_.extract(entity);

      return sandbox_utils::OperationResult::SUCCESS;
    }

    T* GetComponent(Entity entity)
    {
      if (entity_to_index_.find(entity) == entity_to_index_.end())
      {
        return 0;
      }

      return &data_vector_[entity_to_index_[entity]].data;
    }

    std::tuple<StorageItem<T>*, size_t, sandbox_utils::OperationResult> GetComponents()
    {
      return std::make_tuple(data_vector_.data(), data_vector_.size(), sandbox_utils::OperationResult::SUCCESS);
    }

    void EntityDestroyed(Entity entity) override
    {
      if (entity_to_index_.find(entity) != entity_to_index_.end())
      {
        RemoveComponent(entity);
      }
    }

   private:
    std::vector<StorageItem<T>> data_vector_;
    std::unordered_map<Entity, size_t> entity_to_index_;
    std::queue<size_t> free_indices_;
  };
}
