#include <Control/Global.h>
#include <Control/Actions.h>

#include <cstdio>

using namespace Control;

Global::Global( bool allowDevKeys) {
	_allowDevKeys = allowDevKeys;
	if(_allowDevKeys) {
		LevelEditor = new Level::Editor();
	}
}

void Global::onAction(int action_code) {
	if (_allowDevKeys) {
		if (action_code == CONTROL_ACTION_TOGGLE_EDITOR_MODE) {
			printf("Toggled editor mode\n");
			LevelEditor->toggle();
		}
	}
}