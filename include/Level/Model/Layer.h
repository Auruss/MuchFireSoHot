#pragma once

#include <Common/BaseModel.h>
#include <Common/ModelAttribute.h>

#include <Level/Renderer/Layer.h>

namespace Level {
	namespace Model {

		class Layer : public Common::BaseModel {
		
		public:
			Common::ModelAttribute<int> X, Y;
			Common::ModelAttribute<float> Scale;

			Renderer::Layer* Renderer;
		};

	}
}