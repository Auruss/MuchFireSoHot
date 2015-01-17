#pragma once

#include <Level/Renderer/Layer.h>

namespace Level {
	namespace Model { class Base; }

	namespace Renderer {


		/**
			Level Renderer
		*/
		class Base {

		public:
			Model::Base* pAssocModel;

		public:
			Base(Model::Base* model);
			~Base();

			void update();
			void render();

		private:

		};

	}
}