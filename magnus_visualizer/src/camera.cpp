#define _USE_MATH_DEFINES
#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera() : fov(45.0f), aspect(16.0f/9.0f), nearPlane(0.1f), farPlane(500.0f), mouseLookEnabled(true) {
    position = glm::vec3(-10.0f, 3.0f, 30.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::apply() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(position.x, position.y, position.z,
              position.x + forward.x, position.y + forward.y, position.z + forward.z,
              up.x, up.y, up.z);
}

void Camera::rotate(float yaw, float pitch) {
    float yawRad = yaw * static_cast<float>(M_PI) / 180.0f;
    float pitchRad = pitch * static_cast<float>(M_PI) / 180.0f;
    
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yawRad, up) * glm::rotate(glm::mat4(1.0f), pitchRad, right);
    forward = glm::normalize(rotation * glm::vec4(forward, 0.0f));
    right = glm::normalize(glm::cross(forward, up));
}

void Camera::move(float amount) {
    position += forward * amount;
    if (position.y < 0.5f) position.y = 0.5f;
}

void Camera::strafe(float amount) {
    position += right * amount;
    if (position.y < 0.5f) position.y = 0.5f;
}

void Camera::moveUp(float amount) {
    position.y += amount;
    if (position.y < 0.5f) position.y = 0.5f;
}

void Camera::handleMouseMove(int dx, int dy) {
    if (!mouseLookEnabled) return;
    rotate(dx * 0.1f, dy * 0.1f);
}

void Camera::toggleMouseLook() {
    mouseLookEnabled = !mouseLookEnabled;
}
