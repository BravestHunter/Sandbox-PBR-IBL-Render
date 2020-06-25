#pragma once

#include "Sandbox/platform/i_platform_input.h"
#include "Sandbox/platform/glfw/glfw_window.h"

namespace sandbox_platform
{
  class GlfwInput : public IPlatformInput
  {
   public:
    void SetAllInputHandlers(std::shared_ptr<GlfwWindow> window);
    void SetKeyboardKeyInputHandler(std::shared_ptr<GlfwWindow> window);
    void SetKeyboardCharInputHandler(std::shared_ptr<GlfwWindow> window);
    void SetMouseCursorInputHandler(std::shared_ptr<GlfwWindow> window);
    void SetMouseButtonInputHandler(std::shared_ptr<GlfwWindow> window);
    void SetMouseScrollInputHandler(std::shared_ptr<GlfwWindow> window);

    void RemoveAllInputHandlers(std::shared_ptr<GlfwWindow> window);
    void RemoveKeyboardKeyInputHandler(std::shared_ptr<GlfwWindow> window);
    void RemoveKeyboardCharInputHandler(std::shared_ptr<GlfwWindow> window);
    void RemoveMouseCursorInputHandler(std::shared_ptr<GlfwWindow> window);
    void RemoveMouseButtonInputHandler(std::shared_ptr<GlfwWindow> window);
    void RemoveMouseScrollInputHandler(std::shared_ptr<GlfwWindow> window);

    virtual void UpdateInput() override;
    virtual std::tuple<int, int> GetMouseCursorLocation() const override;

   private:
    static sandbox_utils::InputAction ConvertGlfwAction(int glfw_key_action);
    static sandbox_utils::KeybordKey ConvertGlfwKeyboardKey(int glfw_keyboard_key);
    static sandbox_utils::MouseButton ConvertGlfwMouseButton(int glfw_mouse_button);
  };
}
