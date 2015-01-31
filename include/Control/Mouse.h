#pragma once

#include <GLFW/glfw3.h>

#define CONTROL_MOUSE_DRAG_BEGIN    0
#define CONTROL_MOUSE_DRAG_ONGOING  1
#define CONTROL_MOUSE_DRAG_END      2

namespace Control {
  namespace Mouse {
      typedef void (*onSingleClick)(void);
      typedef void (*onDrag)(int, int, int, int);

      extern int X;
      extern int Y;

      extern void setOnSingleClick(onSingleClick event);
      extern void setOnDrag(onDrag event);

      extern void glfw_mouse_move(GLFWwindow* wnd, double x, double y);
      extern void glfw_mouse_button(GLFWwindow* wnd, int button, int action, int mods);
  }
};