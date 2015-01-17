#pragma once

#include <GLFW/glfw3.h>

#include <Level/Editor.h>

namespace Control {

class Global {

public:
	Global(bool allowDevKeys = false);

	void onAction(int action_code);

	Level::Editor* LevelEditor;

private:
	bool _allowDevKeys;

};

}