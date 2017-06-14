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
    std::string contents((std::istreambuf_iterator<char>(fs)),
                       std::istreambuf_iterator<char>());
    return contents;
}

GLuint Shader::compile(GLenum type, const std::string& filename) {
    auto src = readFile(filename);
    auto src_cstr = src.c_str();
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src_cstr, NULL);
    glCompileShader(shader);

    GLint status;
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
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(static_cast<unsigned int>(length, ' '));
        glGetProgramInfoLog(program, length, NULL, &message[0]);
        std::cout << message.size();
        throw std::runtime_error(std::string(message.begin(), message.end()));
    }
    std::cout << "Linked!\n";
    return program;
}

Shader::Shader(const std::string& vsFilename, const std::string& fsFilename) {
    mVert = compile(GL_VERTEX_SHADER, vsFilename);
    mFrag = compile(GL_FRAGMENT_SHADER, fsFilename);
    std::cout << "vert " << mVert << " frag " << mFrag;
    mProgram = link(mVert, mFrag);

}

void Shader::use() {
    glUseProgram(mProgram);
}

GLint Shader::getUniformLocation(const std::string &name) {
    return glGetUniformLocation(mProgram, name.c_str());
}

Shader::~Shader() {
    if(glIsProgram(mProgram)) {
        glDeleteProgram(mProgram);
    }
    if(glIsShader(mVert)) {
        glDeleteShader(mVert);
    }
    if(glIsShader(mFrag)) {
        glDeleteShader(mFrag);
    }
}