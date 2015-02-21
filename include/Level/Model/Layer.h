#pragma once

#include <Common/BaseModel.h>
#include <Common/ModelAttribute.h>

#include <Level/Renderer/Layer.h>

namespace Level {
	namespace Model {

		class Layer : public Common::BaseModel {
		
		public:
			Common::ModelAttribute<int> X, Y, Z;
			Common::ModelAttribute<int> Width, Height;
            Common::ModelAttribute<glm::vec4> Color;
            Common::ModelAttribute<GLuint> Texture;
            Common::ModelAttribute<glm::vec2> TextureCoord;

			Renderer::Layer* Renderer;
		};

	}
}