#pragma once

#include <glm/glm.hpp>

namespace OpenGL {
namespace Helper {
	/**
		Compiles and links vertex and fragment shader
		
		@result program id or -1 if build or link failed (see stderr for more info)
	*/
	extern unsigned int createProgramFromMemory(const char* vertex, const char* fragment);


    extern unsigned int createTextureFromFile(const char* file, glm::vec2& size);

}
}