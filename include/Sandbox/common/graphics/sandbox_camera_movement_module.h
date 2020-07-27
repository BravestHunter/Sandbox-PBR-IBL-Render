#pragma once

#include <memory>
#include <set>

#include "Sandbox/utils/input.h"
#include "Sandbox/common/i_module.h"

namespace sandbox_common {
  class SandboxCameraMovementModule : public IModule
  {
  public:
    SandboxCameraMovementModule(float speed = 5.0f, float mouse_sensitivity = 0.1f) : IModule(), speed_(speed), mouse_sensitivity_(mouse_sensitivity) {}
    virtual ~SandboxCameraMovementModule() = default;

    virtual void Construct() override;
    virtual void Destruct() override;
    virtual void Tick(float delta_time) override;

  private:
    std::set<sandbox_utils::KeybordKey> pressed_keyboard_keys_;
    std::set<sandbox_utils::MouseButton> pressed_mouse_buttons_;
    bool first_move_;
    float last_x_;
    float last_y_;
    float speed_;
    float mouse_sensitivity_;

    short int keyboard_key_listener_;
    short int mouse_cursor_listener_;
    short int mouse_button_listener_;

    void KeyboardKeyEventHandler(const sandbox_utils::KeyboardKeyEvent &e);
    void MouseCursorEventHandler(const sandbox_utils::MouseCursorEvent &e);
    void MouseButtonEventHandler(const sandbox_utils::MouseButtonEvent &e);
  };
}
