#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"



bool keyW = false, keyA = false, keyS = false, keyD = false;
float pitch = 0.0f, yaw = 0.0f;


void framebufferSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key(GLFWwindow* window, int key, int scanCode, int action, int mods) {

    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_W) keyW = true;
        if(key == GLFW_KEY_S) keyS = true;
        if(key == GLFW_KEY_A) keyA = true;
        if(key == GLFW_KEY_D) keyD = true;
        if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
    } else if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_W)	keyW = false;
        if(key == GLFW_KEY_S)	keyS = false;
        if(key == GLFW_KEY_A)	keyA = false;
        if(key == GLFW_KEY_D)	keyD = false;
    }
}

void cursorPos(GLFWwindow* window, double xPos, double yPos) {
    yaw = 512 - xPos;
    pitch = 384 - yPos;
    glfwSetCursorPos(window, 512, 384);
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
    glfwSetFramebufferSizeCallback(window, framebufferSize);
    glfwSetKeyCallback(window, key);
    glfwSetCursorPos(window, 512, 384);

    glfwSetCursorPosCallback(window, cursorPos);

    Camera camera(glm::vec3(0,0, 3));
    camera.pan(glm::radians(180.0f));

    const glm::vec2 SCREEN_SIZE(1024, 768);
    const auto ratio = SCREEN_SIZE.x / SCREEN_SIZE.y;

    auto projectionMatrix = camera.getProjectionMatrix();
    auto modelMatrix = glm::mat4(1.f);


    try {


        Shader flatRed("../media/passthrough.vert", "../media/textured.frag");


        float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,    1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f    // top left
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        flatRed.use();
        glUniformMatrix4fv(flatRed.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(flatRed.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glEnable(GL_DEPTH_TEST);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        auto data = stbi_load("../media/dog.png", &width, &height, &channels, 0);
        if(!data) {
            throw std::runtime_error("Failed to load texture");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);


        auto lastFrame = glfwGetTime();
        while(!glfwWindowShouldClose(window)) {
            auto delta = glfwGetTime() - lastFrame;
            lastFrame = glfwGetTime();

            glfwPollEvents();


            if(keyW) camera.dolly(delta * 1.0f);
            if(keyA) camera.truck(delta * -1.0);
            if(keyS) camera.dolly(delta * -1.0);
            if(keyD) camera.truck(delta * 1.0);

            camera.tilt(pitch / 200.0f);
            camera.pan(yaw / 200.0f);
            pitch = yaw = 0;


            glClearColor(1.0f, 0.933f, 0.678f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            flatRed.use();

            auto viewMatrix = camera.getViewMatrix();
            glUniformMatrix4fv(flatRed.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glfwSwapBuffers(window);
        }
        glDeleteTextures(1, &texture);

    } catch (const std::exception &error) {
        std::cout << "Exception: " << error.what();
    }


    glfwTerminate();
}
