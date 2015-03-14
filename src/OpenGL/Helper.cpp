#include <OpenGL/Helper.h>
#include <GL/glew.h>

#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

#include <SDL/SDL_image.h>
#include <Common/LiveLog/Builder.h>
#include <fstream>

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
    //glDetachShader(program, vs);
    //glDetachShader(program, fs);
    if(!checkProgramLinkStatus(program)) return -1;

    return program;
}

unsigned int OpenGL::Helper::createProgramFromFiles(const char *name) {
    std::string vertex_name = "data/compiled/shader/";
    vertex_name += name;
    vertex_name += "/vertex.glsl";

    std::ifstream file(vertex_name.c_str(), std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> vertex_buffer(size);
    if (!file.read(vertex_buffer.data(), size)) {
        fprintf(stderr, "Failed io operation for vertex shader %s\n", vertex_name.c_str());
        return -1;
    }
    vertex_buffer.push_back('\0');
    file.close();

    std::string frag_name = "data/compiled/shader/";
    frag_name += name;
    frag_name += "/fragment.glsl";

    std::ifstream file2(frag_name.c_str(), std::ios::binary);
    file2.seekg(0, std::ios::end);
    size = file2.tellg();
    file2.seekg(0, std::ios::beg);

    std::vector<char> frag_buffer(size);
    if (!file2.read(frag_buffer.data(), size)) {
        fprintf(stderr, "Failed io operation for frag shader %s\n", frag_name.c_str());
        return -1;
    }
    frag_buffer.push_back('\0');

    return createProgramFromMemory(vertex_buffer.data(), frag_buffer.data());
}
// #######################################################################################################

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