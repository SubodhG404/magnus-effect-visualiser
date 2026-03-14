#include "simulation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <cmath>
#include <iostream>

Simulation::Simulation(Ball& b) : ball(b), 
                                   referenceBall(),
                                   airDensity(1.225f), 
                                   dt(1.0f/60.0f), 
                                   timeScale(1.0f),
                                   magnusVisibility(1.0f),
                                   isLaunched(false), 
                                   showReference(true),
                                   debugFirstFrame(true) {
    gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    currentMagnusForce = glm::vec3(0.0f);
    currentDragForce = glm::vec3(0.0f);
    currentGravityForce = glm::vec3(0.0f);
    referenceVelocity = glm::vec3(0.0f);
    referenceBall.mass = ball.mass;
    referenceBall.radius = ball.radius;
}

void Simulation::reset() {
    ball.reset();
    isLaunched = false;
    currentMagnusForce = glm::vec3(0.0f);
    currentDragForce = glm::vec3(0.0f);
    currentGravityForce = glm::vec3(0.0f);
    referenceVelocity = glm::vec3(0.0f);
}

void Simulation::launch(float initialSpeed, const glm::vec3& spin) {
    reset();
    ball.velocity = glm::vec3(initialSpeed, 0.0f, 0.0f);
    ball.angularVelocity = spin;
    isLaunched = true;
    referenceVelocity = ball.velocity;
}

void Simulation::launchWithVelocity(const glm::vec3& velocity, const glm::vec3& spin) {
    reset();
    ball.velocity = velocity;
    ball.angularVelocity = spin;
    isLaunched = true;
    referenceVelocity = ball.velocity;
    referenceBall.copyFrom(ball);
    referenceBall.mass = ball.mass;
    referenceBall.radius = ball.radius;
    referenceBall.angularVelocity = glm::vec3(0.0f); // No spin for reference
    debugFirstFrame = true;
}

void Simulation::update() {
    if (!isLaunched) return;

    float velocityMagnitude = glm::length(ball.velocity);
    float spinMagnitude = glm::length(ball.angularVelocity);
    
    currentGravityForce = ball.mass * gravity;
    
    // Magnus Force
    glm::vec3 magnusForce = glm::vec3(0.0f);
    if (velocityMagnitude > 0.01f && spinMagnitude > 0.01f) {
        glm::vec3 crossProduct = glm::cross(ball.velocity, ball.angularVelocity);
        
        float crossSectionalArea = 3.14159f * ball.radius * ball.radius;
        float spinParameter = (ball.radius * spinMagnitude) / velocityMagnitude;
        float Cl = 0.25f * spinParameter;
        float magnusMagnitude = 0.5f * airDensity * crossSectionalArea * Cl * velocityMagnitude * velocityMagnitude;
        magnusMagnitude *= magnusVisibility;
        
        if (glm::length(crossProduct) > 0.001f) {
            magnusForce = glm::normalize(crossProduct) * magnusMagnitude;
        }
    }
    currentMagnusForce = magnusForce;
    
    // Drag Force: F = 0.5 * ρ * A * Cd * v^2
    // Simplified with constant Cd for stability
    glm::vec3 dragForce = glm::vec3(0.0f);
    if (velocityMagnitude > 0.01f) {
        float Cd = 0.3f; // Very low drag for visibility
        float crossSectionalArea = 3.14159f * ball.radius * ball.radius;
        float dragMagnitude = 0.5f * airDensity * crossSectionalArea * Cd * velocityMagnitude * velocityMagnitude;
        dragForce = -glm::normalize(ball.velocity) * dragMagnitude;
    }
    currentDragForce = dragForce;

    glm::vec3 totalForce = currentGravityForce + dragForce + magnusForce;
    glm::vec3 acceleration = totalForce / ball.mass;

    float scaledDt = dt * timeScale;
    ball.velocity += acceleration * scaledDt;
    ball.position += ball.velocity * scaledDt;
    ball.updateRotation(scaledDt);
    
    // Update reference ball (without Magnus force)
    float refVelMag = glm::length(referenceBall.velocity);
    glm::vec3 refDragForce = glm::vec3(0.0f);
    if (refVelMag > 0.01f) {
        float Cd = 0.3f;
        float crossSectionalArea = 3.14159f * referenceBall.radius * referenceBall.radius;
        float dragMagnitude = 0.5f * airDensity * crossSectionalArea * Cd * refVelMag * refVelMag;
        refDragForce = -glm::normalize(referenceBall.velocity) * dragMagnitude;
    }
    glm::vec3 refTotalForce = currentGravityForce + refDragForce;
    glm::vec3 refAcceleration = refTotalForce / referenceBall.mass;
    referenceBall.velocity += refAcceleration * scaledDt;
    referenceBall.position += referenceBall.velocity * scaledDt;
    referenceBall.updateRotation(scaledDt);

    if (ball.position.y <= ball.radius) {
        ball.position.y = ball.radius;
        isLaunched = false;
    }
}

float Simulation::getMagnusForceMagnitude() const {
    return glm::length(currentMagnusForce);
}

float Simulation::getDragForceMagnitude() const {
    return glm::length(currentDragForce);
}

float Simulation::getSpeed() const {
    return glm::length(ball.velocity);
}
