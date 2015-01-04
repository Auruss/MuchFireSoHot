#pragma once

#include <Level/Model/Base.h>

namespace Level {
	namespace Renderer {
		class Base {

		public:
			Model::Base* pAssocModel;

		public:
			Base(Model::Base** model);

			void tick();

		private:

		};

	}
}