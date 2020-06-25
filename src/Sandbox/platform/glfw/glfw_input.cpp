#include "Sandbox/platform/glfw/glfw_input.h"

#include "Sandbox/platform/platform_provider.h"

namespace sandbox_platform
{
  void GlfwInput::SetAllInputHandlers(std::shared_ptr<GlfwWindow> window)
  {
    SetKeyboardKeyInputHandler(window);
    SetKeyboardCharInputHandler(window);
    SetMouseCursorInputHandler(window);
    SetMouseButtonInputHandler(window);
    SetMouseScrollInputHandler(window);
  }

  void GlfwInput::SetKeyboardKeyInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    auto keyboard_key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      sandbox_utils::KeyboardKeyEvent event =
      {
        ConvertGlfwKeyboardKey(key),
        ConvertGlfwAction(action),
        mods & GLFW_MOD_SHIFT,
        mods & GLFW_MOD_CONTROL,
        mods & GLFW_MOD_ALT,
        mods & GLFW_MOD_SUPER,
        mods & GLFW_MOD_CAPS_LOCK,
        mods & GLFW_MOD_NUM_LOCK
      };

      PlatformProvider::GetPlatform()->GetPlatformInput().lock()->GenerateKeyboardKeyEvent(event);
    };

    glfwSetKeyCallback(window->GetGlfwWindow(), keyboard_key_callback);
  }

  void GlfwInput::SetKeyboardCharInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    auto keyboard_char_callback = [](GLFWwindow* window, unsigned int codepoint) {
      sandbox_utils::KeyboardCharEvent event =
      {
        codepoint
      };

      PlatformProvider::GetPlatform()->GetPlatformInput().lock()->GenerateKeyboardCharEvent(event);
    };

    glfwSetCharCallback(window->GetGlfwWindow(), keyboard_char_callback);
  }

  void GlfwInput::SetMouseCursorInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    auto mouse_cursor_callback = [](GLFWwindow* window, double xpos, double ypos) {
      sandbox_utils::MouseCursorEvent event =
      {
        xpos,
        ypos
      };

      PlatformProvider::GetPlatform()->GetPlatformInput().lock()->GenerateMouseCursorEvent(event);
    };

    glfwSetCursorPosCallback(window->GetGlfwWindow(), mouse_cursor_callback);
  }

  void GlfwInput::SetMouseButtonInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods) {
      sandbox_utils::MouseButtonEvent event =
      {
        ConvertGlfwMouseButton(button),
        ConvertGlfwAction(action),
        mods & GLFW_MOD_SHIFT,
        mods & GLFW_MOD_CONTROL,
        mods & GLFW_MOD_ALT,
        mods & GLFW_MOD_SUPER,
        mods & GLFW_MOD_CAPS_LOCK,
        mods & GLFW_MOD_NUM_LOCK
      };

      PlatformProvider::GetPlatform()->GetPlatformInput().lock()->GenerateMouseButtonEvent(event);
    };

    glfwSetMouseButtonCallback(window->GetGlfwWindow(), mouse_button_callback);
  }

  void GlfwInput::SetMouseScrollInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    auto mouse_scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset) {
      sandbox_utils::MouseScrollEvent event =
      {
        xoffset,
        yoffset
      };

      PlatformProvider::GetPlatform()->GetPlatformInput().lock()->GenerateMouseScrollEvent(event);
    };

    glfwSetScrollCallback(window->GetGlfwWindow(), mouse_scroll_callback);
  }

  void GlfwInput::RemoveAllInputHandlers(std::shared_ptr<GlfwWindow> window)
  {
    RemoveKeyboardKeyInputHandler(window);
    RemoveKeyboardCharInputHandler(window);
    RemoveMouseCursorInputHandler(window);
    RemoveMouseButtonInputHandler(window);
    RemoveMouseScrollInputHandler(window);
  }

  void GlfwInput::RemoveKeyboardKeyInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    glfwSetKeyCallback(window->GetGlfwWindow(), nullptr);
  }

  void GlfwInput::RemoveKeyboardCharInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    glfwSetCharCallback(window->GetGlfwWindow(), nullptr);
  }

  void GlfwInput::RemoveMouseCursorInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    glfwSetCursorPosCallback(window->GetGlfwWindow(), nullptr);
  }

  void GlfwInput::RemoveMouseButtonInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    glfwSetMouseButtonCallback(window->GetGlfwWindow(), nullptr);
  }

  void GlfwInput::RemoveMouseScrollInputHandler(std::shared_ptr<GlfwWindow> window)
  {
    glfwSetScrollCallback(window->GetGlfwWindow(), nullptr);
  }

  void GlfwInput::UpdateInput()
  {
    glfwPollEvents();
  }

  std::tuple<int, int> GlfwInput::GetMouseCursorLocation() const
  {
    const auto window = PlatformProvider::GetPlatform()->GetPlatformWindow();
    if (window.expired())
      return { -1, -1 }; // todo: change fail return

    const auto gl_window = std::static_pointer_cast<GlfwWindow>(window.lock())->GetGlfwWindow();

    double xpos, ypos;
    glfwGetCursorPos(gl_window, &xpos, &ypos);

    return { (int)xpos, (int)ypos };
  }

  sandbox_utils::InputAction GlfwInput::ConvertGlfwAction(int glfw_key_action)
  {
    switch (glfw_key_action)
    {
    case GLFW_RELEASE:
      return sandbox_utils::InputAction::RELEASE;
    case GLFW_PRESS:
      return sandbox_utils::InputAction::PRESS;
    case GLFW_REPEAT:
      return sandbox_utils::InputAction::REPEAT;
    default:
      return sandbox_utils::InputAction::UNKNOWN;
    }
  }

  sandbox_utils::KeybordKey GlfwInput::ConvertGlfwKeyboardKey(int glfw_keyboard_key)
  {
    switch (glfw_keyboard_key)
    {
      case GLFW_KEY_SPACE:
        return sandbox_utils::KeybordKey::KEY_SPACE;
      case GLFW_KEY_APOSTROPHE:
        return sandbox_utils::KeybordKey::KEY_APOSTROPHE;
      case GLFW_KEY_COMMA:
        return sandbox_utils::KeybordKey::KEY_COMMA;
      case GLFW_KEY_MINUS:
        return sandbox_utils::KeybordKey::KEY_MINUS;
      case GLFW_KEY_PERIOD:
        return sandbox_utils::KeybordKey::KEY_PERIOD;
      case GLFW_KEY_SLASH:
        return sandbox_utils::KeybordKey::KEY_SLASH;
      case GLFW_KEY_0:
        return sandbox_utils::KeybordKey::KEY_0;
      case GLFW_KEY_1:
        return sandbox_utils::KeybordKey::KEY_1;
      case GLFW_KEY_2:
        return sandbox_utils::KeybordKey::KEY_2;
      case GLFW_KEY_3:
        return sandbox_utils::KeybordKey::KEY_3;
      case GLFW_KEY_4:
        return sandbox_utils::KeybordKey::KEY_4;
      case GLFW_KEY_5:
        return sandbox_utils::KeybordKey::KEY_5;
      case GLFW_KEY_6:
        return sandbox_utils::KeybordKey::KEY_6;
      case GLFW_KEY_7:
        return sandbox_utils::KeybordKey::KEY_7;
      case GLFW_KEY_8:
        return sandbox_utils::KeybordKey::KEY_8;
      case GLFW_KEY_9:
        return sandbox_utils::KeybordKey::KEY_9;
      case GLFW_KEY_SEMICOLON:
        return sandbox_utils::KeybordKey::KEY_SEMICOLON;
      case GLFW_KEY_EQUAL:
        return sandbox_utils::KeybordKey::KEY_EQUAL;
      case GLFW_KEY_A:
        return sandbox_utils::KeybordKey::KEY_A;
      case GLFW_KEY_B:
        return sandbox_utils::KeybordKey::KEY_B;
      case GLFW_KEY_C:
        return sandbox_utils::KeybordKey::KEY_C;
      case GLFW_KEY_D:
        return sandbox_utils::KeybordKey::KEY_D;
      case GLFW_KEY_E:
        return sandbox_utils::KeybordKey::KEY_E;
      case GLFW_KEY_F:
        return sandbox_utils::KeybordKey::KEY_F;
      case GLFW_KEY_G:
        return sandbox_utils::KeybordKey::KEY_G;
      case GLFW_KEY_H:
        return sandbox_utils::KeybordKey::KEY_H;
      case GLFW_KEY_I:
        return sandbox_utils::KeybordKey::KEY_I;
      case GLFW_KEY_J:
        return sandbox_utils::KeybordKey::KEY_J;
      case GLFW_KEY_K:
        return sandbox_utils::KeybordKey::KEY_K;
      case GLFW_KEY_L:
        return sandbox_utils::KeybordKey::KEY_L;
      case GLFW_KEY_M:
        return sandbox_utils::KeybordKey::KEY_M;
      case GLFW_KEY_N:
        return sandbox_utils::KeybordKey::KEY_N;
      case GLFW_KEY_O:
        return sandbox_utils::KeybordKey::KEY_O;
      case GLFW_KEY_P:
        return sandbox_utils::KeybordKey::KEY_P;
      case GLFW_KEY_Q:
        return sandbox_utils::KeybordKey::KEY_Q;
      case GLFW_KEY_R:
        return sandbox_utils::KeybordKey::KEY_R;
      case GLFW_KEY_S:
        return sandbox_utils::KeybordKey::KEY_S;
      case GLFW_KEY_T:
        return sandbox_utils::KeybordKey::KEY_T;
      case GLFW_KEY_U:
        return sandbox_utils::KeybordKey::KEY_U;
      case GLFW_KEY_V:
        return sandbox_utils::KeybordKey::KEY_V;
      case GLFW_KEY_W:
        return sandbox_utils::KeybordKey::KEY_W;
      case GLFW_KEY_X:
        return sandbox_utils::KeybordKey::KEY_X;
      case GLFW_KEY_Y:
        return sandbox_utils::KeybordKey::KEY_Y;
      case GLFW_KEY_Z:
        return sandbox_utils::KeybordKey::KEY_Z;
      case GLFW_KEY_LEFT_BRACKET:
        return sandbox_utils::KeybordKey::KEY_LEFT_BRACKET;
      case GLFW_KEY_BACKSLASH:
        return sandbox_utils::KeybordKey::KEY_BACKSLASH;
      case GLFW_KEY_RIGHT_BRACKET:
        return sandbox_utils::KeybordKey::KEY_RIGHT_BRACKET;
      case GLFW_KEY_GRAVE_ACCENT:
        return sandbox_utils::KeybordKey::KEY_GRAVE_ACCENT;
      case GLFW_KEY_WORLD_1:
        return sandbox_utils::KeybordKey::KEY_WORLD1;
      case GLFW_KEY_WORLD_2:
        return sandbox_utils::KeybordKey::KEY_WORLD2;
      case GLFW_KEY_ESCAPE:
        return sandbox_utils::KeybordKey::KEY_ESCSPE;
      case GLFW_KEY_ENTER:
        return sandbox_utils::KeybordKey::KEY_ENTER;
      case GLFW_KEY_TAB:
        return sandbox_utils::KeybordKey::KEY_TAB;
      case GLFW_KEY_BACKSPACE:
        return sandbox_utils::KeybordKey::KEY_BACKSPACE;
      case GLFW_KEY_INSERT:
        return sandbox_utils::KeybordKey::KEY_INSERT;
      case GLFW_KEY_DELETE:
        return sandbox_utils::KeybordKey::KEY_DELETE;
      case GLFW_KEY_RIGHT:
        return sandbox_utils::KeybordKey::KEY_RIGHT;
      case GLFW_KEY_LEFT:
        return sandbox_utils::KeybordKey::KEY_LEFT;
      case GLFW_KEY_DOWN:
        return sandbox_utils::KeybordKey::KEY_DOWN;
      case GLFW_KEY_UP:
        return sandbox_utils::KeybordKey::KEY_UP;
      case GLFW_KEY_PAGE_UP:
        return sandbox_utils::KeybordKey::KEY_PAGE_UP;
      case GLFW_KEY_PAGE_DOWN:
        return sandbox_utils::KeybordKey::KEY_PAGE_DOWN;
      case GLFW_KEY_HOME:
        return sandbox_utils::KeybordKey::KEY_HOME;
      case GLFW_KEY_END:
        return sandbox_utils::KeybordKey::KEY_END;
      case GLFW_KEY_CAPS_LOCK:
        return sandbox_utils::KeybordKey::KEY_CAPS_LOCK;
      case GLFW_KEY_SCROLL_LOCK:
        return sandbox_utils::KeybordKey::KEY_SCROLL_LOCK;
      case GLFW_KEY_NUM_LOCK:
        return sandbox_utils::KeybordKey::KEY_NUM_LOCK;
      case GLFW_KEY_PRINT_SCREEN:
        return sandbox_utils::KeybordKey::KEY_PRINT_SCREEN;
      case GLFW_KEY_PAUSE:
        return sandbox_utils::KeybordKey::KEY_PAUSE;
      case GLFW_KEY_F1:
        return sandbox_utils::KeybordKey::KEY_F1;
      case GLFW_KEY_F2:
        return sandbox_utils::KeybordKey::KEY_F2;
      case GLFW_KEY_F3:
        return sandbox_utils::KeybordKey::KEY_F3;
      case GLFW_KEY_F4:
        return sandbox_utils::KeybordKey::KEY_F4;
      case GLFW_KEY_F5:
        return sandbox_utils::KeybordKey::KEY_F5;
      case GLFW_KEY_F6:
        return sandbox_utils::KeybordKey::KEY_F6;
      case GLFW_KEY_F7:
        return sandbox_utils::KeybordKey::KEY_F7;
      case GLFW_KEY_F8:
        return sandbox_utils::KeybordKey::KEY_F8;
      case GLFW_KEY_F9:
        return sandbox_utils::KeybordKey::KEY_F9;
      case GLFW_KEY_F10:
        return sandbox_utils::KeybordKey::KEY_F10;
      case GLFW_KEY_F11:
        return sandbox_utils::KeybordKey::KEY_F11;
      case GLFW_KEY_F12:
        return sandbox_utils::KeybordKey::KEY_F12;
      case GLFW_KEY_F13:
        return sandbox_utils::KeybordKey::KEY_F13;
      case GLFW_KEY_F14:
        return sandbox_utils::KeybordKey::KEY_F14;
      case GLFW_KEY_F15:
        return sandbox_utils::KeybordKey::KEY_F15;
      case GLFW_KEY_F16:
        return sandbox_utils::KeybordKey::KEY_F16;
      case GLFW_KEY_F17:
        return sandbox_utils::KeybordKey::KEY_F17;
      case GLFW_KEY_F18:
        return sandbox_utils::KeybordKey::KEY_F18;
      case GLFW_KEY_F19:
        return sandbox_utils::KeybordKey::KEY_F19;
      case GLFW_KEY_F20:
        return sandbox_utils::KeybordKey::KEY_F20;
      case GLFW_KEY_F21:
        return sandbox_utils::KeybordKey::KEY_F21;
      case GLFW_KEY_F22:
        return sandbox_utils::KeybordKey::KEY_F22;
      case GLFW_KEY_F23:
        return sandbox_utils::KeybordKey::KEY_F23;
      case GLFW_KEY_F24:
        return sandbox_utils::KeybordKey::KEY_F24;
      case GLFW_KEY_F25:
        return sandbox_utils::KeybordKey::KEY_F25;
      case GLFW_KEY_KP_0:
        return sandbox_utils::KeybordKey::KEY_KP0;
      case GLFW_KEY_KP_1:
        return sandbox_utils::KeybordKey::KEY_KP1;
      case GLFW_KEY_KP_2:
        return sandbox_utils::KeybordKey::KEY_KP2;
      case GLFW_KEY_KP_3:
        return sandbox_utils::KeybordKey::KEY_KP3;
      case GLFW_KEY_KP_4:
        return sandbox_utils::KeybordKey::KEY_KP4;
      case GLFW_KEY_KP_5:
        return sandbox_utils::KeybordKey::KEY_KP5;
      case GLFW_KEY_KP_6:
        return sandbox_utils::KeybordKey::KEY_KP6;
      case GLFW_KEY_KP_7:
        return sandbox_utils::KeybordKey::KEY_KP7;
      case GLFW_KEY_KP_8:
        return sandbox_utils::KeybordKey::KEY_KP8;
      case GLFW_KEY_KP_9:
        return sandbox_utils::KeybordKey::KEY_KP9;
      case GLFW_KEY_KP_DECIMAL:
        return sandbox_utils::KeybordKey::KEY_KP_DECIMAL;
      case GLFW_KEY_KP_DIVIDE:
        return sandbox_utils::KeybordKey::KEY_KP_DIVIDE;
      case GLFW_KEY_KP_MULTIPLY:
        return sandbox_utils::KeybordKey::KEY_KP_MULTIPLY;
      case GLFW_KEY_KP_SUBTRACT:
        return sandbox_utils::KeybordKey::KEY_KP_SUBTRACT;
      case GLFW_KEY_KP_ADD:
        return sandbox_utils::KeybordKey::KEY_KP_ADD;
      case GLFW_KEY_KP_ENTER:
        return sandbox_utils::KeybordKey::KEY_KP_ENTER;
      case GLFW_KEY_KP_EQUAL:
        return sandbox_utils::KeybordKey::KEY_KP_EQUAL;
      case GLFW_KEY_LEFT_SHIFT:
        return sandbox_utils::KeybordKey::KEY_LEFT_SHIFT;
      case GLFW_KEY_LEFT_CONTROL:
        return sandbox_utils::KeybordKey::KEY_LEFT_CONTROL;
      case GLFW_KEY_LEFT_ALT:
        return sandbox_utils::KeybordKey::KEY_LEFT_ALT;
      case GLFW_KEY_LEFT_SUPER:
        return sandbox_utils::KeybordKey::KEY_LEFT_SUPER;
      case GLFW_KEY_RIGHT_SHIFT:
        return sandbox_utils::KeybordKey::KEY_RIGHT_SHIFT;
      case GLFW_KEY_RIGHT_CONTROL:
        return sandbox_utils::KeybordKey::KEY_RIGHT_CONTROL;
      case GLFW_KEY_RIGHT_ALT:
        return sandbox_utils::KeybordKey::KEY_RIGHT_ALT;
      case GLFW_KEY_RIGHT_SUPER:
        return sandbox_utils::KeybordKey::KEY_RIGHT_SUPER;
      case GLFW_KEY_MENU:
        return sandbox_utils::KeybordKey::KEY_MENU;
      default:
        return sandbox_utils::KeybordKey::KEY_UNKNOWN;
    }
  }

  sandbox_utils::MouseButton GlfwInput::ConvertGlfwMouseButton(int glfw_mouse_button)
  {
    switch (glfw_mouse_button)
    {
      case GLFW_MOUSE_BUTTON_1:
        return sandbox_utils::MouseButton::BUTTON_1;
      case GLFW_MOUSE_BUTTON_2:
        return sandbox_utils::MouseButton::BUTTON_2;
      case GLFW_MOUSE_BUTTON_3:
        return sandbox_utils::MouseButton::BUTTON_3;
      case GLFW_MOUSE_BUTTON_4:
        return sandbox_utils::MouseButton::BUTTON_4;
      case GLFW_MOUSE_BUTTON_5:
        return sandbox_utils::MouseButton::BUTTON_5;
      case GLFW_MOUSE_BUTTON_6:
        return sandbox_utils::MouseButton::BUTTON_6;
      case GLFW_MOUSE_BUTTON_7:
        return sandbox_utils::MouseButton::BUTTON_7;
      case GLFW_MOUSE_BUTTON_8:
        return sandbox_utils::MouseButton::BUTTON_8;
      default:
        return sandbox_utils::MouseButton::UNKNOWN;
    }
  }
}
