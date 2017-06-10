#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaders.h"

void framebuffer_resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 1024, 768);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize);



    auto vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vShader);

    int status;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        char message[512];
        glGetShaderInfoLog(vShader, 512, NULL, message);
        std::cout << "Failed to compiled vertex shader\n" << message << '\n';
    }

    auto fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        char message[512];
        glGetShaderInfoLog(fShader, 512, NULL, message);
        std::cout << "Failed to compiled fragment shader\n" << message << '\n';
    }

    auto shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    glGetShaderiv(shader, GL_LINK_STATUS, &status);
    if(!status) {
        char message[512];
        glGetProgramInfoLog(shader, 512, NULL, message);
        std::cout << "Failed to link program\n" << message << '\n';
    }

    float vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1.0f, 0.933f, 0.678f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    glDeleteProgram(shader);

    glfwTerminate();
}
