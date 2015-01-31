#include <Control/Mouse.h>

using namespace Control;

int Mouse::X = 0;
int Mouse::Y = 0;

Mouse::onSingleClick _single_click = nullptr;
Mouse::onDrag _drag = nullptr;

int _mod = 0;
bool _clicking = false;
bool _dragging = false;
int _dragX = 0;
int _dragY = 0;

void Mouse::setOnSingleClick(onSingleClick event) {
    _single_click = event;
}

void Mouse::setOnDrag(onDrag event) {
    _drag = event;
}

void Mouse::glfw_mouse_button(GLFWwindow *wnd, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) {
            _mod = mods;
            _clicking = true;
            _dragX = X;
            _dragY = Y;
        }
        else if(action == GLFW_RELEASE) {
            _clicking = false;
            if(_dragging) {
                _drag(_dragX, _dragY, CONTROL_MOUSE_DRAG_END, _mod);
            }
            else {
                _single_click();
            }
            _dragging = false;
        }
    }
}

void Mouse::glfw_mouse_move(GLFWwindow *wnd, double x, double y) {
    X = (int)x;
    Y = (int)y;

    if(_clicking) {
        if(_dragging) {
            _drag(_dragX, _dragY, CONTROL_MOUSE_DRAG_ONGOING, _mod);
        } else {
            _drag(_dragX, _dragY, CONTROL_MOUSE_DRAG_BEGIN, _mod);
            _dragging = true;
        }

        _dragX = X;
        _dragY = Y;
    }
}