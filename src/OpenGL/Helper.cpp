#include <OpenGL/Helper.h>
#include <GL/glew.h>

#include <vector>
#include <string>
#include <cstdio>

#include <SDL/SDL_image.h>

bool checkShaderCompileStatus(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE) {
		return true;
	} 
	else {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		fprintf(stderr, "Shader failed: %s\n", buffer);
		return false;
	}
}

bool checkProgramLinkStatus(GLuint program) {
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *) &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		fprintf(stderr, "Linking failed: %s\n", infoLog.data());
		return false;
	}
	return true;
}

unsigned int OpenGL::Helper::createProgramFromMemory(const char *vertex, const char *fragment) {
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	int vs_len = strlen(vertex);
	int fs_len = strlen(fragment);

	glShaderSource(vs, 1, &vertex, &vs_len);
	glShaderSource(fs, 1, &fragment, &fs_len);

	glCompileShader(vs);
	if(!checkShaderCompileStatus(vs)) return -1;
	glCompileShader(fs);
	if(!checkShaderCompileStatus(fs)) return -1;

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	if(!checkProgramLinkStatus(program)) return -1;

	return program; 
}

#define IS_POW_2(x) (((x != 0) && ((x & (~x + 1)) == x)))

unsigned int OpenGL::Helper::createTextureFromFile(const char *file, glm::vec2& size) {
    auto surface = IMG_Load(file);
    if(!IS_POW_2(surface->w) || !IS_POW_2(surface->h)) {
        fprintf(stderr, "'%s' resolutions are not power of 2\n", file);
        return -1;
    }

	size.x = surface->w;
	size.y = surface->h;

    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w,surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}