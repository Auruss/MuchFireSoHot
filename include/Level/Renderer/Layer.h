#pragma once


#include <OpenGL/RenderSystem.h>

namespace Level {
	namespace Model { class Layer; }

	namespace Renderer {
		class Layer {
		public:
			Layer(Model::Layer* layer);
			
			void update();
			void render();

		protected:
			Model::Layer* _model;

		private:
			OpenGL::RenderSystem::Combined _buffer;


			void full_vertices_update();
            void position_update();
            void color_update();
            void index_update();
		};
	}
}