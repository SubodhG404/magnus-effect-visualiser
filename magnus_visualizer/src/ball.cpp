#include "ball.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Standard soccer ball: mass 0.43kg, radius 11cm
// Initial position: on ground at origin
Ball::Ball() : mass(0.43f), radius(0.11f), rotationAngle(0.0f), rotationAxis(1.0f, 0.0f, 0.0f) {
    position = glm::vec3(0.0f, 0.11f, 0.0f);
    velocity = glm::vec3(0.0f);
    angularVelocity = glm::vec3(0.0f);
}

// Reset to initial position
void Ball::reset() {
    position = glm::vec3(0.0f, radius, 0.0f);
    velocity = glm::vec3(0.0f);
    angularVelocity = glm::vec3(0.0f);
    rotationAngle = 0.0f;
    rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
}

// Update rotation angle based on spin
// Convert angular velocity (rad/s) to degrees for OpenGL
void Ball::updateRotation(float dt) {
    float spinMag = glm::length(angularVelocity);
    if (spinMag > 0.001f) {
        rotationAngle += spinMag * dt * 180.0f / 3.14159f;
        rotationAxis = glm::normalize(angularVelocity);
    }
}

// Get normalized spin direction
glm::vec3 Ball::getSpinDirection() const {
    if (glm::length(angularVelocity) > 0.001f) {
        return glm::normalize(angularVelocity);
    }
    return glm::vec3(0.0f);
}

// Get spin magnitude in rad/s
float Ball::getSpinMagnitude() const {
    return glm::length(angularVelocity);
}

// Copy all properties from another ball
void Ball::copyFrom(const Ball& other) {
    position = other.position;
    velocity = other.velocity;
    angularVelocity = other.angularVelocity;
    mass = other.mass;
    radius = other.radius;
    rotationAngle = other.rotationAngle;
    rotationAxis = other.rotationAxis;
}
