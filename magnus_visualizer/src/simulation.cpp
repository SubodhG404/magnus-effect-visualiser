#include "simulation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <cmath>

static const float PI = 3.14159f;
static const float AIR_DENSITY = 1.225f;      // kg/m^3 at sea level
static const float AIR_VISCOSITY = 1.81e-5f;  // Pa.s

// Constructor: initialize physics constants and ball properties
Simulation::Simulation(Ball& b) : ball(b), 
                                   referenceBall(),
                                   airDensity(AIR_DENSITY), 
                                   dt(1.0f/60.0f), 
                                   timeScale(1.0f),
                                   airViscosity(AIR_VISCOSITY),
                                   isLaunched(false) {
    gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    currentMagnusForce = glm::vec3(0.0f);
    currentDragForce = glm::vec3(0.0f);
    currentGravityForce = glm::vec3(0.0f);
    referenceBall.mass = ball.mass;
    referenceBall.radius = ball.radius;
}

float Simulation::calculateDragCoefficient(float velocityMagnitude) {
    float reynoldsNumber = (2.0f * ball.radius * airDensity * velocityMagnitude) / airViscosity;
    if (reynoldsNumber < 1.0f) return 24.0f / reynoldsNumber;
    if (reynoldsNumber < 1000.0f) return 24.0f / reynoldsNumber + 0.4f;
    if (reynoldsNumber < 200000.0f) {
        float cd = 0.5f * (1.0f - std::tanh(0.2f * (std::log10(reynoldsNumber) - 2.0f)));
        if (cd < 0.2f) cd = 0.2f;
        return cd;
    }
    return 0.4f;
}

float Simulation::calculateLiftCoefficient(float spinMagnitude, float velocityMagnitude) {
    float spinParameter = (ball.radius * spinMagnitude) / velocityMagnitude;
    float cl = 0.4f * spinParameter;
    if (cl > 0.6f) cl = 0.6f;
    return cl;
}

void Simulation::reset() {
    ball.reset();
    isLaunched = false;
    currentMagnusForce = glm::vec3(0.0f);
    currentDragForce = glm::vec3(0.0f);
    currentGravityForce = glm::vec3(0.0f);
    referenceBall.reset();
}

void Simulation::launch(float initialSpeed, const glm::vec3& spin) {
    reset();
    ball.velocity = glm::vec3(initialSpeed, 0.0f, 0.0f);
    ball.angularVelocity = spin;
    isLaunched = true;
}

void Simulation::launchWithVelocity(const glm::vec3& velocity, const glm::vec3& spin) {
    reset();
    ball.velocity = velocity;
    ball.angularVelocity = spin;
    isLaunched = true;
    referenceBall.copyFrom(ball);
    referenceBall.mass = ball.mass;
    referenceBall.radius = ball.radius;
    referenceBall.angularVelocity = glm::vec3(0.0f);
}

void Simulation::update() {
    if (!isLaunched) return;

    float velocityMagnitude = glm::length(ball.velocity);
    float spinMagnitude = glm::length(ball.angularVelocity);
    float crossSectionalArea = PI * ball.radius * ball.radius;
    
    currentGravityForce = ball.mass * gravity;
    
    // Magnus force: F = 0.5 * rho * A * Cl * v^2
    // Direction: omega x v (perpendicular to spin and velocity)
    glm::vec3 magnusForce = glm::vec3(0.0f);
    if (velocityMagnitude > 0.01f && spinMagnitude > 0.01f) {
        glm::vec3 crossProduct = glm::cross(ball.angularVelocity, ball.velocity);
        float Cl = calculateLiftCoefficient(spinMagnitude, velocityMagnitude);
        float magnusMagnitude = 0.5f * airDensity * crossSectionalArea * Cl * velocityMagnitude * velocityMagnitude;
        
        if (glm::length(crossProduct) > 0.001f) {
            magnusForce = glm::normalize(crossProduct) * magnusMagnitude;
        }
    }
    currentMagnusForce = magnusForce;
    
    // Drag force: F = 0.5 * rho * A * Cd * v^2 (opposes velocity)
    glm::vec3 dragForce = glm::vec3(0.0f);
    if (velocityMagnitude > 0.01f) {
        float Cd = calculateDragCoefficient(velocityMagnitude);
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
    
    // Update reference ball (no spin = no magnus force)
    float refVelMag = glm::length(referenceBall.velocity);
    float refArea = PI * referenceBall.radius * referenceBall.radius;
    glm::vec3 refDragForce = glm::vec3(0.0f);
    if (refVelMag > 0.01f) {
        float Cd = calculateDragCoefficient(refVelMag);
        float dragMagnitude = 0.5f * airDensity * refArea * Cd * refVelMag * refVelMag;
        refDragForce = -glm::normalize(referenceBall.velocity) * dragMagnitude;
    }
    glm::vec3 refGravityForce = referenceBall.mass * gravity;
    glm::vec3 refTotalForce = refGravityForce + refDragForce;
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
