//
// Created by adam on 6/12/17.
//
#include <fstream>
#include <iostream>
#include <vector>
#include "Shader.h"

std::string Shader::readFile(const std::string& filename) {
    std::ifstream fs(filename);
    if(!fs) {
        throw std::runtime_error(filename + " not found!");
    }
    return std::string((std::istreambuf_iterator<char>(fs)),
                       std::istreambuf_iterator<char>());
}

GLuint Shader::compile(GLenum type, const std::string& filename) {
    auto src = readFile(filename).c_str();
    std::cout << src << '\n';
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(static_cast<unsigned int>(length));
        glGetShaderInfoLog(shader, length, NULL, &message[0]);
        throw std::runtime_error(std::string(message.begin(), message.end()));
    }
    std::cout << "Compiled " << filename << "\n";
    return shader;
}

GLuint Shader::link(GLuint vert, GLuint frag) {
    auto program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    int status;
    glGetShaderiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        GLint length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(static_cast<unsigned int>(length));
        glGetProgramInfoLog(program, length, NULL, &message[0]);
        throw std::runtime_error(std::string(message.begin(), message.end()));
    }
    return program;
}

Shader::Shader(const std::string& vsFilename, const std::string& fsFilename) {
    vert = compile(GL_VERTEX_SHADER, vsFilename);
    frag = compile(GL_FRAGMENT_SHADER, fsFilename);
    program = link(vert, frag);

}

void Shader::use() {
    glUseProgram(program);
}

Shader::~Shader() {
    if(glIsProgram(program)) {
        glDeleteProgram(program);
    }
    if(glIsShader(vert)) {
        glDeleteShader(vert);
    }
    if(glIsShader(frag)) {
        glDeleteShader(frag);
    }
}