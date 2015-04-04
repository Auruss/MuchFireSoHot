#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Level/Editor.h>

namespace Control {

class Global {

public:
	Global(bool allowDevKeys = false);

	void onAction(int action_code);
    void onSingleClick();
    void onDrag(int startX, int startY, int state, int mods);

	Level::Editor* LevelEditor;

private:
	bool _allowDevKeys;

};

}