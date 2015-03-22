#pragma once

#include <Level/Model/Base.h>
#include <OpenGL/RenderSystem.h>
#include <OpenGL/Global.h>

extern "C" void editor_update_vals(int type);

namespace Level {
	class Editor {
	public:
		Editor();
        ~Editor();

		void toggle();

		void onClick();
		void onDrag(int x, int y, int state, int mods);

        void updatePositions();
        void updateJsPositions();
        void render();

		void setCurrentLevel(Model::Base* lvl);
        void setCurrentLight(Light::Model* light);
        void setCurrentLayer(Model::Layer* layer);

		Model::Base* getCurrentLevel() { return _current_level; }
        Model::Layer* getCurrentLayer() { return _current_layer; }
        Light::Model* getCurrentLight() { return _current_light; }

        bool isEditing();


		static Editor* Instance;

	private:
        void initial_buffer();

        bool _isActivated;

        int _current_type;
		Model::Base* _current_level;
        Model::Layer* _current_layer;
        Light::Model* _current_light;

        OpenGL::RenderSystem::Combined _buffer;

        OpenGL::RenderSystem* _render_system;
        Storage::GpuBuffer<glm::vec3> _vertex_buffer;
        Storage::GpuBuffer<glm::vec4> _color_buffer;
        Storage::GpuBuffer<unsigned int> _index_buffer;
        unsigned int _color_program;
	};
}