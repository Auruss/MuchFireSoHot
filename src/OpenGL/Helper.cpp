#include <OpenGL/Helper.h>
#include <GL/glew.h>

#include <vector>
#include <string>
#include <cstdio>

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