#pragma once

#include <Level/Model/Base.h>
#include <OpenGL/RenderSystem.h>

extern "C" void editor_update_vals(int type);

namespace Level {
	class Editor {
	public:
		Editor();

		void toggle();

		void onClick();
		void onDrag(int x, int y, int state, int mods);

        void updatePositions();
        void updateJsPositions();
        void render();

		void setCurrentLevel(Model::Base* lvl);
        void setCurrentLayer(Model::Layer* layer);

		Model::Base* getCurrentLevel() { return _current_level; }
        Model::Layer* getCurrentLayer() { return _current_layer; }


		static Editor* Instance;

	private:
        void initial_buffer();

        bool _isActivated;

		Model::Base* _current_level;
        Model::Layer* _current_layer;

        OpenGL::RenderSystem::Combined _buffer;
	};
}