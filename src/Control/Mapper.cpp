#include <Control/Mapper.h>
#include <Control/Actions.h>

using namespace Control;

Mapper::Mapper() {

}

int Mapper::mapGLFWinput(int key_code, int mode, int mod) {
	// CTRL + D
	if (key_code == GLFW_KEY_M && mode == GLFW_RELEASE) {
		return CONTROL_ACTION_TOGGLE_EDITOR_MODE;
	}

	return CONTROL_ACTION_UNKNOWN;
}