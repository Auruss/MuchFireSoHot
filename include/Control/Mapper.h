#pragma once


#include <GLFW/glfw3.h>

namespace Control {

class Mapper {
public:
	Mapper();

	int mapGLFWinput(int key_code, int mode, int mod);
};

}
