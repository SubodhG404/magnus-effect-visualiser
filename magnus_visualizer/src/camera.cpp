#define _USE_MATH_DEFINES
#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Fly camera: free movement in 3D space, mouse look
// Default position: behind/above ball start, looking toward origin
Camera::Camera() : fov(45.0f), aspect(16.0f/9.0f), nearPlane(0.1f), farPlane(500.0f), mouseLookEnabled(true) {
    position = glm::vec3(-10.0f, 3.0f, 30.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

// Apply camera to OpenGL: set projection and modelview matrices
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

// Rotate camera: yaw around up axis, pitch around right axis
void Camera::rotate(float yaw, float pitch) {
    float yawRad = yaw * static_cast<float>(M_PI) / 180.0f;
    float pitchRad = pitch * static_cast<float>(M_PI) / 180.0f;
    
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yawRad, up) * glm::rotate(glm::mat4(1.0f), pitchRad, right);
    forward = glm::normalize(rotation * glm::vec4(forward, 0.0f));
    right = glm::normalize(glm::cross(forward, up));
}

// Move forward/backward in viewing direction
void Camera::move(float amount) {
    position += forward * amount;
    if (position.y < 0.5f) position.y = 0.5f; // Prevent going below ground
}

// Strafe left/right perpendicular to view direction
void Camera::strafe(float amount) {
    position += right * amount;
    if (position.y < 0.5f) position.y = 0.5f;
}

// Move up/down (vertical)
void Camera::moveUp(float amount) {
    position.y += amount;
    if (position.y < 0.5f) position.y = 0.5f;
}

// Mouse movement: rotate camera when mouse look is enabled
void Camera::handleMouseMove(int dx, int dy) {
    if (!mouseLookEnabled) return;
    rotate(dx * 0.1f, dy * 0.1f);
}

// Toggle mouse look on/off with spacebar
void Camera::toggleMouseLook() {
    mouseLookEnabled = !mouseLookEnabled;
}
