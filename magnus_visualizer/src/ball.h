#ifndef BALL_H
#define BALL_H

#include <glm/glm.hpp>

class Ball {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 angularVelocity;
    float mass;
    float radius;
    float rotationAngle;
    glm::vec3 rotationAxis;

    Ball();

    void reset();
    void updateRotation(float dt);

    glm::vec3 getSpinDirection() const;
    float getSpinMagnitude() const;
    
    void copyFrom(const Ball& other);
};

#endif
