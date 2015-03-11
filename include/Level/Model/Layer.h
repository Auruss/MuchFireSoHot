#pragma once

#include <Common/BaseModel.h>
#include <Common/ModelAttribute.h>

#include <Level/Renderer/Layer.h>

namespace Level {
	namespace Model {
        class Base;

		class Layer : public Common::BaseModel {
		
		public:
			Common::ModelAttribute<int> X, Y, Z;
			Common::ModelAttribute<int> Width, Height;
            Common::ModelAttribute<glm::vec4> TextureCoord;
            Common::ModelAttribute<float> Rotation;

			Renderer::Layer* Renderer;
            Model::Base* Base;
		};

	}
}