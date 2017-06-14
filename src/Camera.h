//
// Created by adam on 6/12/17.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

class Camera {

    glm::vec3 mPosition, mDirection;
    float mZoom;
    glm::mat4 mProjection, mView;

    void rotate(const float amount, const glm::vec3& axis) {
        mDirection = glm::rotate(mDirection, amount, axis);
    }

    void updateProjectionMatrix() {
        mProjection = glm::perspective(glm::radians(mZoom), 4.0f / 3.0f, 0.1f, 100.0f);
    }

    void updateViewMatrix() {
        mView = glm::lookAt(mPosition, mPosition + mDirection, glm::vec3(0, 1, 0));
    }

public:

    void dolly(const float amount) {
        mPosition += mDirection * amount;
        updateViewMatrix();
    }

    void truck(const float amount) {
        mPosition += glm::cross(mDirection, glm::vec3(0, 1, 0)) * amount;
        updateViewMatrix();
    }

    void pan(const float amount) {
        rotate(amount, glm::vec3(0, 1, 0));
        updateViewMatrix();
    }

    void tilt(const float amount) {
        auto right = glm::cross(mDirection, glm::vec3(0, 1, 0));
        rotate(amount, right);
        updateViewMatrix();
    }

    void zoom(const float amount) {
        mZoom += amount;
        mZoom = std::min(50.f, std::max(1.0f, mZoom));
        updateProjectionMatrix();
    }

    Camera(const glm::vec3& position) :
            mPosition(position) {

        mZoom = 45.0f;
        mDirection = glm::vec3(0, 0, 1);
        updateProjectionMatrix();
        updateViewMatrix();
    }

    const glm::mat4& getProjectionMatrix() const {
        return mProjection;
    }

    const glm::mat4& getViewMatrix() const {
        return mView;
    }


};


#endif //RENDERER_CAMERA_H
