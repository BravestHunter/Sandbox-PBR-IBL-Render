#pragma once

#include <queue>
#include <set>
#include <unordered_map>

#include "Sandbox/common/i_module.h"
#include "Sandbox/utils/operation_result.h"
#include "Sandbox/common/scene/scene_base.h"
#include "Sandbox/common/scene/component_storage.h"

namespace sandbox_common
{
  class SceneModule : public IModule
  {
   public:
    SceneModule(Entity max_entities = 1000);

    ~SceneModule();

    virtual void Construct() override;
    virtual void Destruct() override;
    virtual void Tick(float delta_time) override;

    sandbox_utils::OperationResult CreateEntity(Entity &entity);
    sandbox_utils::OperationResult RemoveEntity(Entity entity);

    template<typename T>
    sandbox_utils::OperationResult RegisterComponent()
    {
      ComponentType type = typeid(T).hash_code();
      if (registered_components_.find(type) != registered_components_.end())
      {
        sandbox_utils::OperationResult::FAILURE;
      }

      registered_components_.insert(type);
      component_storages_.insert({ type, std::make_shared<ComponentStorage<T>>() });
      return sandbox_utils::OperationResult::SUCCESS;
    }

    template<typename T>
    sandbox_utils::OperationResult AddComponent(Entity entity, T component)
    {
      ComponentType type = typeid(T).hash_code();
      if (registered_components_.find(type) == registered_components_.end())
      {
        sandbox_utils::OperationResult::FAILURE;
      }

      return GetComponentStorage<T>(type)->InsertComponent(entity, component);
    }

    template<typename T>
    T* GetComponent(Entity entity)
    {
      ComponentType type = typeid(T).hash_code();
      if (registered_components_.find(type) == registered_components_.end())
      {
        return 0;
      }

      return GetComponentStorage<T>(type)->GetComponent(entity);
    }

    template<typename T>
    sandbox_utils::OperationResult GetComponents(ComponentStorage<T>::StorageItem<T>** componentsRef, size_t* components_count)
    {
      ComponentType type = typeid(T).hash_code();
      if (registered_components_.find(type) == registered_components_.end())
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      std::tuple<ComponentStorage<T>::StorageItem<T>*, size_t, sandbox_utils::OperationResult> result = GetComponentStorage<T>(type)->GetComponents();
      *componentsRef = std::get<0>(result);
      *components_count = std::get<1>(result);

      return std::get<2>(result);
    }

    template<typename T>
    sandbox_utils::OperationResult RemoveComponent(Entity entity)
    {
      ComponentType type = typeid(T).hash_code();
      if (registered_components_.find(type) == registered_components_.end())
      {
        return sandbox_utils::OperationResult::FAILURE;
      }

      return GetComponentStorage<T>(type)->RemoveComponent(entity);
    }

    private:
     Entity max_entities_;
     std::queue<Entity> available_entities_{};
     std::set<Entity> existing_entities_{};

     std::set<ComponentType> registered_components_{};
     std::unordered_map<ComponentType, std::shared_ptr<IComponentStorage>> component_storages_{};

     template<typename T>
     std::shared_ptr<ComponentStorage<T>> GetComponentStorage(ComponentType type)
     {
       return std::static_pointer_cast<ComponentStorage<T>>(component_storages_[type]);
     }
  };
}