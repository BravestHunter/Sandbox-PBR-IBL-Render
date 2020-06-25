#include "Sandbox/platform/i_platform_input.h"

namespace sandbox_platform
{
  sandbox_utils::OperationResult IPlatformInput::AddKeyboardKeyEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::KeyboardKeyEvent&)> &callback, short int &id)
  {
    if (owner.expired())
      sandbox_utils::OperationResult::FAILURE;

    return on_keyboard_key_event_.Bind(sandbox_utils::Delegate<void, const sandbox_utils::KeyboardKeyEvent&>::CreateWeakDelegate(owner, callback), id);
  }

  sandbox_utils::OperationResult IPlatformInput::AddKeyboardCharEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::KeyboardCharEvent&)> &callback, short int &id)
  {
    if (owner.expired())
      sandbox_utils::OperationResult::FAILURE;

    return on_keyboard_text_event_.Bind(sandbox_utils::Delegate<void, const sandbox_utils::KeyboardCharEvent&>::CreateWeakDelegate(owner, callback), id);
  }

  sandbox_utils::OperationResult IPlatformInput::AddMouseCursorEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseCursorEvent&)> &callback, short int &id)
  {
    if (owner.expired())
      sandbox_utils::OperationResult::FAILURE;

    return on_mouse_cursor_event_.Bind(sandbox_utils::Delegate<void, const sandbox_utils::MouseCursorEvent&>::CreateWeakDelegate(owner, callback), id);
  }

  sandbox_utils::OperationResult IPlatformInput::AddMouseButtonEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseButtonEvent&)> &callback, short int &id)
  {
    if (owner.expired())
      sandbox_utils::OperationResult::FAILURE;

    return on_mouse_button_event_.Bind(sandbox_utils::Delegate<void, const sandbox_utils::MouseButtonEvent&>::CreateWeakDelegate(owner, callback), id);
  }

  sandbox_utils::OperationResult IPlatformInput::AddMouseScrollEventListener(const std::weak_ptr<void> &owner, const std::function<void(const sandbox_utils::MouseScrollEvent&)> &callback, short int &id)
  {
    if (owner.expired())
      sandbox_utils::OperationResult::FAILURE;

    return on_mouse_scroll_event_.Bind(sandbox_utils::Delegate<void, const sandbox_utils::MouseScrollEvent&>::CreateWeakDelegate(owner, callback), id);
  }

  sandbox_utils::OperationResult IPlatformInput::RemoveKeyboardKeyEventListener(short int delegate_id)
  {
    return on_keyboard_key_event_.Remove(delegate_id);
  }

  sandbox_utils::OperationResult IPlatformInput::RemoveKeyboardCharEventListener(short int delegate_id)
  {
    return on_keyboard_text_event_.Remove(delegate_id);
  }

  sandbox_utils::OperationResult IPlatformInput::RemoveMouseCursorEventListener(short int delegate_id)
  {
    return on_mouse_cursor_event_.Remove(delegate_id);
  }

  sandbox_utils::OperationResult IPlatformInput::RemoveMouseButtonEventListener(short int delegate_id)
  {
    return on_mouse_button_event_.Remove(delegate_id);
  }

  sandbox_utils::OperationResult IPlatformInput::RemoveMouseScrollEventListener(short int delegate_id)
  {
    return on_mouse_scroll_event_.Remove(delegate_id);
  }

  void IPlatformInput::GenerateKeyboardKeyEvent(const sandbox_utils::KeyboardKeyEvent &e)
  {
    on_keyboard_key_event_.InvokeAll(e);
  }

  void IPlatformInput::GenerateKeyboardCharEvent(const sandbox_utils::KeyboardCharEvent &e)
  {
    on_keyboard_text_event_.InvokeAll(e);
  }

  void IPlatformInput::GenerateMouseCursorEvent(const sandbox_utils::MouseCursorEvent &e)
  {
    on_mouse_cursor_event_.InvokeAll(e);
  }

  void IPlatformInput::GenerateMouseButtonEvent(const sandbox_utils::MouseButtonEvent &e)
  {
    on_mouse_button_event_.InvokeAll(e);
  }

  void IPlatformInput::GenerateMouseScrollEvent(const sandbox_utils::MouseScrollEvent &e)
  {
    on_mouse_scroll_event_.InvokeAll(e);
  }
}
