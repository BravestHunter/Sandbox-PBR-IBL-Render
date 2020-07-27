#include "Sandbox/common/graphics/sandbox_camera_movement_module.h"

#include "Sandbox/common/graphics/opengl/opengl_render_module.h"
#include "Sandbox/common/graphics/camera.h"
#include "Sandbox/platform/platform_provider.h"

namespace sandbox_common
{
  void SandboxCameraMovementModule::Construct()
  {
    auto window_ = sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformWindow().lock();
    auto input = sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock();
    input->AddKeyboardKeyEventListener(window_, [&](const sandbox_utils::KeyboardKeyEvent &e) { this->KeyboardKeyEventHandler(e); }, keyboard_key_listener_);
    input->AddMouseCursorEventListener(window_, [&](const sandbox_utils::MouseCursorEvent &e) { this->MouseCursorEventHandler(e); }, mouse_cursor_listener_);
    input->AddMouseButtonEventListener(window_, [&](const sandbox_utils::MouseButtonEvent &e) { this->MouseButtonEventHandler(e); }, mouse_button_listener_);

    first_move_ = true;
    auto size = window_->GetWindowSize();
    last_x_ = std::get<0>(size);
    last_y_ = std::get<1>(size);

    is_constructed_ = true;
  }

  void SandboxCameraMovementModule::Destruct()
  {
    auto input = sandbox_platform::PlatformProvider::GetPlatform()->GetPlatformInput().lock();
    input->RemoveKeyboardKeyEventListener(keyboard_key_listener_);
    input->RemoveMouseCursorEventListener(mouse_cursor_listener_);
    input->RemoveMouseButtonEventListener(mouse_button_listener_);

    is_constructed_ = false;
  }

  void SandboxCameraMovementModule::Tick(float delta_time)
  {
    if (!is_constructed_)
      return;

    std::weak_ptr<Camera> camera_ = std::static_pointer_cast<OpenglRenderModule>(parent_.lock())->GetCamera();

    if (camera_.expired())
      return;

    if (pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_UP) > 0 || pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_W) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::FORWARD, delta_time * speed_);
    }
    if (pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_DOWN) > 0 || pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_S) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::BACKWARD, delta_time * speed_);
    }
    if (pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_RIGHT) > 0 || pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_D) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::RIGHT, delta_time * speed_);
    }
    if (pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_LEFT) > 0 || pressed_keyboard_keys_.count(sandbox_utils::KeybordKey::KEY_A) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::LEFT, delta_time * speed_);
    }

    if (pressed_mouse_buttons_.count(sandbox_utils::MouseButton::BUTTON_1) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::UP, delta_time * speed_);
    }
    if (pressed_mouse_buttons_.count(sandbox_utils::MouseButton::BUTTON_2) > 0)
    {
      camera_.lock()->ChangePosition(CameraMovement::DOWN, delta_time * speed_);
    }
  }

  void SandboxCameraMovementModule::KeyboardKeyEventHandler(const sandbox_utils::KeyboardKeyEvent &e)
  {
    if (e.action == sandbox_utils::InputAction::PRESS)
    {
      pressed_keyboard_keys_.insert(e.key);
    }
    else if (e.action == sandbox_utils::InputAction::RELEASE)
    {
      pressed_keyboard_keys_.erase(e.key);
    }
  }

  void SandboxCameraMovementModule::MouseCursorEventHandler(const sandbox_utils::MouseCursorEvent &e)
  {
    std::weak_ptr<Camera> camera_ = std::static_pointer_cast<OpenglRenderModule>(parent_.lock())->GetCamera();

    if (camera_.expired())
      return;

    if (first_move_)
    {
      last_x_ = e.x_position;
      last_y_ = e.y_position;
      first_move_ = false;
    }

    GLfloat xoffset = e.x_position - last_x_;
    GLfloat yoffset = last_y_ - e.y_position;

    last_x_ = e.x_position;
    last_y_ = e.y_position;

    camera_.lock()->ChangeDirection(xoffset * mouse_sensitivity_, yoffset * mouse_sensitivity_);
  }

  void SandboxCameraMovementModule::MouseButtonEventHandler(const sandbox_utils::MouseButtonEvent &e)
  {
    if (e.action == sandbox_utils::InputAction::PRESS)
    {
      pressed_mouse_buttons_.insert(e.button);
    }
    else if (e.action == sandbox_utils::InputAction::RELEASE)
    {
      pressed_mouse_buttons_.erase(e.button);
    }
  }
}
