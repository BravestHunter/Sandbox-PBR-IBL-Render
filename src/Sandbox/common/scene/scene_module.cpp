#include "Sandbox/common/scene/scene_module.h"

namespace sandbox_common
{
  SceneModule::SceneModule(Entity max_entities) : max_entities_(max_entities)
  {
    for (Entity entity = 1; entity <= max_entities; ++entity)
    {
      available_entities_.push(entity);
    }
  }

  SceneModule::~SceneModule()
  {
    
  }

  void SceneModule::Construct()
  {
    is_constructed_ = true;
  }

  void SceneModule::Destruct()
  {
    is_constructed_ = false;
  }

  void SceneModule::Tick(float delta_time)
  {

  }

  sandbox_utils::OperationResult SceneModule::CreateEntity(Entity &entity)
  {
    if (available_entities_.size() == 0)
    {
      return sandbox_utils::OperationResult::FAILURE;
    }

    entity = available_entities_.front();
    available_entities_.pop();
    existing_entities_.insert(entity);

    return sandbox_utils::OperationResult::SUCCESS;
  }

  sandbox_utils::OperationResult SceneModule::RemoveEntity(Entity entity)
  {
    if (existing_entities_.find(entity) == existing_entities_.end())
    {
      return sandbox_utils::OperationResult::FAILURE;
    }

    for (auto storage : component_storages_)
    {
      storage.second->EntityDestroyed(entity);
    }

    existing_entities_.erase(entity);
    available_entities_.push(entity);

    return sandbox_utils::OperationResult::SUCCESS;
  }
}
