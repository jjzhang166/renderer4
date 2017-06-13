//
// Created by adam on 6/12/17.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

    glm::vec3 position, forward, up, right;
    float yaw, pitch, zoom, speed, mouseSensitivity;

public:

    Camera(const glm::vec3& position) :
            position(position) {
        up = glm::vec3(0.f, 1.f, 0.f);
        forward = glm::vec3(0.f, 0.f, -1.f);
        yaw = pitch = 0.f;
        zoom = 45.0f;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + forward, up);
    }


};


#endif //RENDERER_CAMERA_H
