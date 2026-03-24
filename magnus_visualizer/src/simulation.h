#ifndef SIMULATION_H
#define SIMULATION_H

#include "ball.h"
#include <glm/glm.hpp>

class Simulation {
public:
    Ball& ball;
    Ball referenceBall;
    
    float airDensity;
    float dt;
    float timeScale;
    float airViscosity;
    
    glm::vec3 gravity;
    glm::vec3 currentMagnusForce;
    glm::vec3 currentDragForce;
    glm::vec3 currentGravityForce;
    bool isLaunched;

    Simulation(Ball& b);

    void reset();
    void resetToPosition(const glm::vec3& startPosition);
    void launchWithVelocity(const glm::vec3& velocity, const glm::vec3& spin);

    void update();
    
    float calculateLiftCoefficient(float spinMagnitude, float velocityMagnitude);
    
    float calculateDragCoefficient(float velocityMagnitude);
    
    float getMagnusForceMagnitude() const;
    
    float getSpeed() const;
};

#endif