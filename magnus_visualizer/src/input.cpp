#include "input.h"
#include <glm/glm.hpp>

Input::Input(Ball& b, Simulation& sim, Camera& cam, Trail& t) 
    : ball(b), simulation(sim), camera(cam), trail(t),
      velocityX(35.0f), velocityY(4.0f), velocityZ(8.0f),
      spinRPM(700.0f), spinAxisX(0.0f), spinAxisY(1.0f), spinAxisZ(-0.28f),
      startPosX(0.0f), startPosY(0.11f), startPosZ(0.0f) {
}

glm::vec3 Input::getCurrentSpin() const {
    if (spinRPM == 0.0f) return glm::vec3(0.0f);
    float rpmToRadSec = 2.0f * 3.14159f / 60.0f;
    glm::vec3 axis(spinAxisX, spinAxisY, spinAxisZ);
    float axisLen = glm::length(axis);
    if (axisLen < 0.001f) return glm::vec3(0.0f);
    return glm::normalize(axis) * spinRPM * rpmToRadSec;
}

void Input::handleMouseDrag(int dx, int dy) {
    camera.handleMouseMove(dx, dy);
}

void Input::launch() {
    trail.clear();
    ball.position = glm::vec3(startPosX, startPosY, startPosZ);
    glm::vec3 velocity(velocityX, velocityY, velocityZ);
    glm::vec3 spin = getCurrentSpin();
    simulation.launchWithVelocity(velocity, spin);
}

void Input::reset() {
    trail.clear();
    simulation.reset();
}