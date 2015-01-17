#pragma once

namespace OpenGL {
namespace Helper {
	/**
		Compiles and links vertex and fragment shader
		
		@result program id or -1 if build or link failed (see stderr for more info)
	*/
	extern unsigned int createProgramFromMemory(const char* vertex, const char* fragment);

}
}