#pragma once

#include <string>

namespace sandbox_utils 
{
  enum class InputAction
  {
    UNKNOWN = -1,
    RELEASE,
    PRESS,
    REPEAT
  };

  enum class KeybordKey
  {
    KEY_UNKNOWN = -1,
    KEY_SPACE,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET,
    KEY_GRAVE_ACCENT,
    KEY_WORLD1,
    KEY_WORLD2,
    KEY_ESCSPE,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,
    KEY_F25,
    KEY_KP0,
    KEY_KP1,
    KEY_KP2,
    KEY_KP3,
    KEY_KP4,
    KEY_KP5,
    KEY_KP6,
    KEY_KP7,
    KEY_KP8,
    KEY_KP9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_MENU
  };

  enum class MouseButton
  {
    UNKNOWN = -1,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8
  };

  struct KeyboardKeyEvent
  {
    KeybordKey key;
    InputAction action;
    bool mod_shift;
    bool mod_control;
    bool mod_alt;
    bool mod_super;
    bool mod_caps_lock;
    bool mod_num_lock;
  };

  struct KeyboardCharEvent
  {
    unsigned int key_code;
  };

  struct MouseCursorEvent
  {
    double x_position;
    double y_position;
  };

  struct MouseButtonEvent
  {
    MouseButton button;
    InputAction action;
    bool mod_shift;
    bool mod_control;
    bool mod_alt;
    bool mod_super;
    bool mod_caps_lock;
    bool mod_num_lock;
  };

  struct MouseScrollEvent
  {
    double x_offset;
    double y_offset;
  };

  static std::string InputActionToString(InputAction action)
  {
    switch (action)
    {
      case InputAction::RELEASE:
        return "Release";
      case InputAction::PRESS:
        return "Press";
      case InputAction::REPEAT:
        return "Repeat";
      case InputAction::UNKNOWN:
      default:
        return "Unknown";
    }
  }

  static std::string KeyboardKeyToString(KeybordKey key)
  {
    switch (key)
    {
      case KeybordKey::KEY_SPACE:
        return "Space";
      case KeybordKey::KEY_APOSTROPHE:
        return "Apostrophe";
      case KeybordKey::KEY_COMMA:
        return "Comma";
      case KeybordKey::KEY_MINUS:
        return "Minus";
      case KeybordKey::KEY_PERIOD:
        return "Period";
      case KeybordKey::KEY_SLASH:
        return "Slash";
      case KeybordKey::KEY_0:
        return "0";
      case KeybordKey::KEY_1:
        return "1";
      case KeybordKey::KEY_2:
        return "2";
      case KeybordKey::KEY_3:
        return "3";
      case KeybordKey::KEY_4:
        return "4";
      case KeybordKey::KEY_5:
        return "5";
      case KeybordKey::KEY_6:
        return "6";
      case KeybordKey::KEY_7:
        return "7";
      case KeybordKey::KEY_8:
        return "8";
      case KeybordKey::KEY_9:
        return "9";
      case KeybordKey::KEY_SEMICOLON:
        return "Semicolon";
      case KeybordKey::KEY_EQUAL:
        return "Equal";
      case KeybordKey::KEY_A:
        return "A";
      case KeybordKey::KEY_B:
        return "B";
      case KeybordKey::KEY_C:
        return "C";
      case KeybordKey::KEY_D:
        return "D";
      case KeybordKey::KEY_E:
        return "E";
      case KeybordKey::KEY_F:
        return "F";
      case KeybordKey::KEY_G:
        return "G";
      case KeybordKey::KEY_H:
        return "H";
      case KeybordKey::KEY_I:
        return "I";
      case KeybordKey::KEY_J:
        return "J";
      case KeybordKey::KEY_K:
        return "K";
      case KeybordKey::KEY_L:
        return "L";
      case KeybordKey::KEY_M:
        return "M";
      case KeybordKey::KEY_N:
        return "N";
      case KeybordKey::KEY_O:
        return "O";
      case KeybordKey::KEY_P:
        return "P";
      case KeybordKey::KEY_Q:
        return "Q";
      case KeybordKey::KEY_R:
        return "R";
      case KeybordKey::KEY_S:
        return "S";
      case KeybordKey::KEY_T:
        return "T";
      case KeybordKey::KEY_U:
        return "U";
      case KeybordKey::KEY_V:
        return "V";
      case KeybordKey::KEY_W:
        return "W";
      case KeybordKey::KEY_X:
        return "X";
      case KeybordKey::KEY_Y:
        return "Y";
      case KeybordKey::KEY_Z:
        return "Z";
      case KeybordKey::KEY_LEFT_BRACKET:
        return "Left Bracket";
      case KeybordKey::KEY_BACKSLASH:
        return "Backslash";
      case KeybordKey::KEY_RIGHT_BRACKET:
        return "Right Bracket";
      case KeybordKey::KEY_GRAVE_ACCENT:
        return "Grave Accent";
      case KeybordKey::KEY_WORLD1:
        return "World 1";
      case KeybordKey::KEY_WORLD2:
        return "World 2";
      case KeybordKey::KEY_ESCSPE:
        return "Escape";
      case KeybordKey::KEY_ENTER:
        return "Enter";
      case KeybordKey::KEY_TAB:
        return "Tab";
      case KeybordKey::KEY_BACKSPACE:
        return "Backspace";
      case KeybordKey::KEY_INSERT:
        return "Insert";
      case KeybordKey::KEY_DELETE:
        return "Delete";
      case KeybordKey::KEY_RIGHT:
        return "Right";
      case KeybordKey::KEY_LEFT:
        return "Left";
      case KeybordKey::KEY_DOWN:
        return "Down";
      case KeybordKey::KEY_UP:
        return "Up";
      case KeybordKey::KEY_PAGE_UP:
        return "Page Up";
      case KeybordKey::KEY_PAGE_DOWN:
        return "Page Down";
      case KeybordKey::KEY_HOME:
        return "Home";
      case KeybordKey::KEY_END:
        return "End";
      case KeybordKey::KEY_CAPS_LOCK:
        return "Caps Lock";
      case KeybordKey::KEY_SCROLL_LOCK:
        return "Scroll Lock";
      case KeybordKey::KEY_NUM_LOCK:
        return "Num Lock";
      case KeybordKey::KEY_PRINT_SCREEN:
        return "Print Screen";
      case KeybordKey::KEY_PAUSE:
        return "Pause";
      case KeybordKey::KEY_F1:
        return "F1";
      case KeybordKey::KEY_F2:
        return "F2";
      case KeybordKey::KEY_F3:
        return "F3";
      case KeybordKey::KEY_F4:
        return "F4";
      case KeybordKey::KEY_F5:
        return "F5";
      case KeybordKey::KEY_F6:
        return "F6";
      case KeybordKey::KEY_F7:
        return "F7";
      case KeybordKey::KEY_F8:
        return "F8";
      case KeybordKey::KEY_F9:
        return "F9";
      case KeybordKey::KEY_F10:
        return "F10";
      case KeybordKey::KEY_F11:
        return "F11";
      case KeybordKey::KEY_F12:
        return "F12";
      case KeybordKey::KEY_F13:
        return "F13";
      case KeybordKey::KEY_F14:
        return "F14";
      case KeybordKey::KEY_F15:
        return "F15";
      case KeybordKey::KEY_F16:
        return "F16";
      case KeybordKey::KEY_F17:
        return "F17";
      case KeybordKey::KEY_F18:
        return "F18";
      case KeybordKey::KEY_F19:
        return "F19";
      case KeybordKey::KEY_F20:
        return "F20";
      case KeybordKey::KEY_F21:
        return "F21";
      case KeybordKey::KEY_F22:
        return "F22";
      case KeybordKey::KEY_F23:
        return "F23";
      case KeybordKey::KEY_F24:
        return "F24";
      case KeybordKey::KEY_F25:
        return "F25";
      case KeybordKey::KEY_KP0:
        return "KP0";
      case KeybordKey::KEY_KP1:
        return "KP1";
      case KeybordKey::KEY_KP2:
        return "KP2";
      case KeybordKey::KEY_KP3:
        return "KP3";
      case KeybordKey::KEY_KP4:
        return "KP4";
      case KeybordKey::KEY_KP5:
        return "KP5";
      case KeybordKey::KEY_KP6:
        return "KP6";
      case KeybordKey::KEY_KP7:
        return "KP7";
      case KeybordKey::KEY_KP8:
        return "KP8";
      case KeybordKey::KEY_KP9:
        return "KP9";
      case KeybordKey::KEY_KP_DECIMAL:
        return "KP Decimal";
      case KeybordKey::KEY_KP_DIVIDE:
        return "KP Divide";
      case KeybordKey::KEY_KP_MULTIPLY:
        return "KP Multiply";
      case KeybordKey::KEY_KP_SUBTRACT:
        return "KP Substract";
      case KeybordKey::KEY_KP_ADD:
        return "KP Add";
      case KeybordKey::KEY_KP_ENTER:
        return "KP Enter";
      case KeybordKey::KEY_KP_EQUAL:
        return "KP Equal";
      case KeybordKey::KEY_LEFT_SHIFT:
        return "Left Shift";
      case KeybordKey::KEY_LEFT_CONTROL:
        return "Left Control";
      case KeybordKey::KEY_LEFT_ALT:
        return "Left Alt";
      case KeybordKey::KEY_LEFT_SUPER:
        return "Left Super";
      case KeybordKey::KEY_RIGHT_SHIFT:
        return "Right Shift";
      case KeybordKey::KEY_RIGHT_CONTROL:
        return "Right Control";
      case KeybordKey::KEY_RIGHT_ALT:
        return "Right Alt";
      case KeybordKey::KEY_RIGHT_SUPER:
        return "Right Super";
      case KeybordKey::KEY_MENU:
        return "Menu";
      case KeybordKey::KEY_UNKNOWN:
      default:
        return "Unknown";
    }
  }

  static std::string MouseButtonToString(MouseButton button)
  {
    switch (button)
    {
      case MouseButton::BUTTON_1:
        return "Button 1";
      case MouseButton::BUTTON_2:
        return "Button 2";
      case MouseButton::BUTTON_3:
        return "Button 3";
      case MouseButton::BUTTON_4:
        return "Button 4";
      case MouseButton::BUTTON_5:
        return "Button 5";
      case MouseButton::BUTTON_6:
        return "Button 6";
      case MouseButton::BUTTON_7:
        return "Button 7";
      case MouseButton::BUTTON_8:
        return "Button 8";
      case MouseButton::UNKNOWN:
      default:
        return "Unknown";
    }
  }
}
