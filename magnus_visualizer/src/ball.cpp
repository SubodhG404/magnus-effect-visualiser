#include "ball.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Ball::Ball() : mass(0.45f), radius(0.11f), rotationAngle(0.0f) {
    reset();
}

void Ball::reset() {
    position = glm::vec3(0.0f, radius, 0.0f);
    velocity = glm::vec3(0.0f);
    angularVelocity = glm::vec3(0.0f);
    rotationAngle = 0.0f;
    rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
}

void Ball::updateRotation(float dt) {
    float spinMag = glm::length(angularVelocity);
    if (spinMag > 0.001f) {
        rotationAngle += spinMag * dt * 180.0f / 3.14159f;
        rotationAxis = glm::normalize(angularVelocity);
    }
}

glm::vec3 Ball::getSpinDirection() const {
    if (glm::length(angularVelocity) > 0.001f) {
        return glm::normalize(angularVelocity);
    }
    return glm::vec3(0.0f);
}

float Ball::getSpinMagnitude() const {
    return glm::length(angularVelocity);
}

void Ball::copyFrom(const Ball& other) {
    position = other.position;
    velocity = other.velocity;
    angularVelocity = other.angularVelocity;
    mass = other.mass;
    radius = other.radius;
    rotationAngle = other.rotationAngle;
    rotationAxis = other.rotationAxis;
}
