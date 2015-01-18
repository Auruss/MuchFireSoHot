#pragma once

#include <Level/Model/Base.h>

extern "C" void editor_update_vals();

namespace Level {
	class Editor {
	public:
		Editor();

		void toggle();

		void onClick(int x, int y, bool right);
		void onDrag(int x, int y, bool right);

		void setCurrentLevel(Model::Base* lvl) {_current_level = lvl; }
		Model::Base* getCurrentLevel() { return _current_level; }

		static Editor* Instance;

	private:
		bool _isActivated;
		Model::Base* _current_level;
	};
}