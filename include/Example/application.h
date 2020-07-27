#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Sandbox/platform/platform_provider.h"
#include "Sandbox/platform/glfw/glfw_platform.h"
#include "Sandbox/platform/glfw/glfw_input.h"

#include "Sandbox/common/i_module.h"
#include "Sandbox/common/sandbox_engine.h"
#include "Sandbox/common/scene/scene_module.h"
#include "Sandbox/common/graphics/sandbox_graphics_module.h"
#include "Sandbox/common/graphics/sandbox_camera_movement_module.h"

#include "Sandbox/common/graphics/sandbox_graphics_component.h"
#include "Sandbox/common/graphics/opengl/opengl_render_module.h"
#include "Sandbox/common/graphics/mesh.h"

#include "Sandbox/common/graphics/opengl/opengl_model_component.h"
#include "Sandbox/graphics/graphics_model.h"

#include "Sandbox/utils/input.h"

class InterruptApplicationModule : public sandbox_common::IModule
{
public:
  InterruptApplicationModule() : sandbox_common::IModule() {}
  virtual ~InterruptApplicationModule() = default;

  virtual void Construct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->AddKeyboardKeyEventListener(
      sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow(),
      [&](const sandbox_utils::KeyboardKeyEvent &e) { this->KeyboardKeyEventHandler(e); }, keyboard_key_listener_);

    is_constructed_ = true;
  }

  virtual void Destruct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->RemoveKeyboardKeyEventListener(keyboard_key_listener_);

    is_constructed_ = false;
  }

  virtual void Tick(float delta_time) override {};

private:
  short int keyboard_key_listener_;

  void KeyboardKeyEventHandler(const sandbox_utils::KeyboardKeyEvent &e)
  {
    if (e.action == sandbox_utils::InputAction::PRESS && e.key == sandbox_utils::KeybordKey::KEY_ESCSPE)
    {
      std::static_pointer_cast<sandbox_common::SandboxEngine>(parent_.lock())->Stop();
    }
  }
};

class ModelWrapModule : public sandbox_common::IModule
{
public:
  ModelWrapModule() : sandbox_common::IModule() {}
  virtual ~ModelWrapModule() = default;

  virtual void Construct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->AddKeyboardKeyEventListener(
      sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow(),
      [&](const sandbox_utils::KeyboardKeyEvent &e) { this->KeyboardKeyEventHandler(e); }, keyboard_key_listener_);

    is_constructed_ = true;
  }

  virtual void Destruct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->RemoveKeyboardKeyEventListener(keyboard_key_listener_);

    is_constructed_ = false;
  }

  virtual void Tick(float delta_time) override 
  {
    if (parent_.expired())
      return;
    std::shared_ptr<sandbox_common::SceneModule> scene = std::static_pointer_cast<sandbox_common::SceneModule>(parent_.lock());
    
    sandbox_common::OpenglModelComponent *component = scene->GetComponent<sandbox_common::OpenglModelComponent>(wrap_entity_);
    if (component == 0)
      return;

    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP7) != pressed_keys_.end())
    {
      component->transformation.rotation *= glm::quat(glm::vec3(0.0f, rotation_speed_, 0.0f));
    }
    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP9) != pressed_keys_.end())
    {
      component->transformation.rotation *= glm::quat(glm::vec3(0.0f, -rotation_speed_, 0.0f));
    }
    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP4) != pressed_keys_.end())
    { 
      component->transformation.rotation *= glm::quat(glm::vec3(0.0f, 0.0f, rotation_speed_));
    }
    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP6) != pressed_keys_.end())
    {
      component->transformation.rotation *= glm::quat(glm::vec3(0.0f, 0.0f, -rotation_speed_));
    }
    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP8) != pressed_keys_.end())
    {
      component->transformation.rotation *= glm::quat(glm::vec3(rotation_speed_, 0.0f, 0.0f));
    }
    if (pressed_keys_.find(sandbox_utils::KeybordKey::KEY_KP2) != pressed_keys_.end())
    {
      component->transformation.rotation *= glm::quat(glm::vec3(-rotation_speed_, 0.0f, 0.0f));
    }
  };

  void SetEntity(sandbox_common::Entity entity)
  {
    wrap_entity_ = entity;
  }

private:
  short int keyboard_key_listener_;
  std::set<sandbox_utils::KeybordKey> pressed_keys_;
  sandbox_common::Entity wrap_entity_;
  float rotation_speed_ = 0.05f;

  void KeyboardKeyEventHandler(const sandbox_utils::KeyboardKeyEvent &e)
  {
    if (e.action == sandbox_utils::InputAction::PRESS)
    {
      pressed_keys_.insert(e.key);
    }
    else if(e.action == sandbox_utils::InputAction::RELEASE)
    {
      pressed_keys_.erase(e.key);
    }
  }
};

class OpenglRenderControlModule : public sandbox_common::IModule
{
public:
  OpenglRenderControlModule() : sandbox_common::IModule() {}
  virtual ~OpenglRenderControlModule() = default;

  virtual void Construct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->AddKeyboardKeyEventListener(
      sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow(),
      [&](const sandbox_utils::KeyboardKeyEvent &e) { this->KeyboardKeyEventHandler(e); }, keyboard_key_listener_);

    is_constructed_ = true;
  }

  virtual void Destruct() override
  {
    sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock()->RemoveKeyboardKeyEventListener(keyboard_key_listener_);

    is_constructed_ = false;
  }

  virtual void Tick(float delta_time) override {};

private:
  short int keyboard_key_listener_;

  void KeyboardKeyEventHandler(const sandbox_utils::KeyboardKeyEvent &e)
  {
    if (e.action == sandbox_utils::InputAction::PRESS)
    {
      if (parent_.expired())
        return;
      std::shared_ptr<sandbox_common::OpenglRenderModule> render_module = std::static_pointer_cast<sandbox_common::OpenglRenderModule>(parent_.lock());

      if (e.key == sandbox_utils::KeybordKey::KEY_1)
      {
        unsigned int mode = render_module->GetPolygonMode();
        if (mode == GL_FILL)
          mode = GL_POINT;
        else
          mode++;

        render_module->SetPolygonMode(mode);
      }

      if (e.key == sandbox_utils::KeybordKey::KEY_2)
      {
        render_module->SetPointLightsMode(!render_module->GetPointLightsMode());
      }
    }
  }
};

class GraphicsApplication {
public:
  static int RunExample1(int width, int height);
  static int RunExample2(int width, int height);
  static int RunExample3(int width, int height);
  static int RunExample4(int width, int height);
  static int RunExample5(int width, int height);
  static int RunExample6(int width, int height);
  static int RunExample7(int width, int height);
  static int RunExample8(int width, int height);
  static int RunExample9(int width, int height);
  static int RunExample10(int width, int height);
  static int RunExample11(int width, int height);
  static int RunExample12(int width, int height);

private:
  static void ConfigurateSystems(int width, int height);
  static sandbox_graphics::GraphicsModel CreateAxisModel();
};