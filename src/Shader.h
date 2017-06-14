//
// Created by adam on 6/12/17.
//

#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader {

    GLuint mVert, mFrag, mProgram;

    std::string readFile(const std::string& filename);
    GLuint compile(GLenum type, const std::string& filename);

    GLuint link(GLuint vert, GLuint frag);

public:
    Shader(const std::string& vsFilename, const std::string& fsFilename);
    void use();
    GLint getUniformLocation(const std::string &name);
    ~Shader();
};


#endif //RENDERER_SHADER_H
