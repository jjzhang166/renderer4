#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Camera.h"

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

    Camera camera(glm::vec3(0,0, 3));

    const glm::vec2 SCREEN_SIZE(1024, 768);
    const auto ratio = SCREEN_SIZE.x / SCREEN_SIZE.y;

    auto projectionMatrix = glm::perspective(glm::radians(45.f), ratio, 0.1f, 100.f);
    auto modelMatrix = glm::mat4(1.f);


    try {
        Shader flatRed("../media/passthrough.vert", "../media/red.frag");

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

        flatRed.use();
        glUniformMatrix4fv(flatRed.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(flatRed.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClearColor(1.0f, 0.933f, 0.678f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            flatRed.use();

            auto viewMatrix = camera.getViewMatrix();
            glUniformMatrix4fv(flatRed.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glfwSwapBuffers(window);
        }

    } catch (const std::exception &error) {
        std::cout << "Exception: " << error.what();
    }


    glfwTerminate();
}
