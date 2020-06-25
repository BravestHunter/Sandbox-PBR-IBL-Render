#pragma once

#include <memory>

#include "Sandbox/utils/input.h"
#include "Sandbox/utils/multicast_delegate.h"
#include "Sandbox/utils/operation_result.h"

namespace sandbox_platform 
{
  class IPlatformInput
  {
   public:
    IPlatformInput() = default;
    virtual ~IPlatformInput() = default;

    sandbox_utils::OperationResult AddKeyboardKeyEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::KeyboardKeyEvent&)> &callback, short int &id);
    sandbox_utils::OperationResult AddKeyboardCharEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::KeyboardCharEvent&)> &callback, short int &id);
    sandbox_utils::OperationResult AddMouseCursorEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseCursorEvent&)> &callback, short int &id);
    sandbox_utils::OperationResult AddMouseButtonEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseButtonEvent&)> &callback, short int &id);
    sandbox_utils::OperationResult AddMouseScrollEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseScrollEvent&)> &callback, short int &id);

    sandbox_utils::OperationResult RemoveKeyboardKeyEventListener(short int delegate_id);
    sandbox_utils::OperationResult RemoveKeyboardCharEventListener(short int delegate_id);
    sandbox_utils::OperationResult RemoveMouseCursorEventListener(short int delegate_id);
    sandbox_utils::OperationResult RemoveMouseButtonEventListener(short int delegate_id);
    sandbox_utils::OperationResult RemoveMouseScrollEventListener(short int delegate_id);

    void GenerateKeyboardKeyEvent(const sandbox_utils::KeyboardKeyEvent &e);
    void GenerateKeyboardCharEvent(const sandbox_utils::KeyboardCharEvent &e);
    void GenerateMouseCursorEvent(const sandbox_utils::MouseCursorEvent &e);
    void GenerateMouseButtonEvent(const sandbox_utils::MouseButtonEvent &e);
    void GenerateMouseScrollEvent(const sandbox_utils::MouseScrollEvent &e);

    virtual void UpdateInput() = 0;

    virtual std::tuple<int, int> GetMouseCursorLocation() const = 0;

   protected:
    sandbox_utils::MulticastDelegate<const sandbox_utils::KeyboardKeyEvent&> on_keyboard_key_event_ = sandbox_utils::MulticastDelegate<const sandbox_utils::KeyboardKeyEvent&>(128);
    sandbox_utils::MulticastDelegate<const sandbox_utils::KeyboardCharEvent&> on_keyboard_text_event_ = sandbox_utils::MulticastDelegate<const sandbox_utils::KeyboardCharEvent&>(64);
    sandbox_utils::MulticastDelegate<const sandbox_utils::MouseCursorEvent&> on_mouse_cursor_event_ = sandbox_utils::MulticastDelegate<const sandbox_utils::MouseCursorEvent&>(64);
    sandbox_utils::MulticastDelegate<const sandbox_utils::MouseButtonEvent&> on_mouse_button_event_ = sandbox_utils::MulticastDelegate<const sandbox_utils::MouseButtonEvent&>(128);
    sandbox_utils::MulticastDelegate<const sandbox_utils::MouseScrollEvent&> on_mouse_scroll_event_ = sandbox_utils::MulticastDelegate<const sandbox_utils::MouseScrollEvent&>(64);
  };
}
