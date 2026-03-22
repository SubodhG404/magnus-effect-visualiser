#include "input.h"
#include <glm/glm.hpp>
#include <iostream>

Input::Input(Ball& b, Simulation& sim, Camera& cam, Vectors& v, Trail& t) 
    : ball(b), simulation(sim), camera(cam), vectors(v), trail(t),
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

void Input::handleKeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            camera.toggleMouseLook();
            break;
        case 'r':
        case 'R':
            reset();
            break;
        case 'v':
        case 'V':
            vectors.enabled = !vectors.enabled;
            break;
        case 't':
        case 'T':
            trail.enabled = !trail.enabled;
            break;
    }
}

void Input::handleSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camera.move(0.5f);
            break;
        case GLUT_KEY_DOWN:
            camera.move(-0.5f);
            break;
        case GLUT_KEY_LEFT:
            camera.strafe(0.5f);
            break;
        case GLUT_KEY_RIGHT:
            camera.strafe(-0.5f);
            break;
    }
}

void Input::handleMouseDrag(int dx, int dy) {
    camera.handleMouseMove(dx, dy);
}

void Input::handleMouseWheel(int wheel, int direction, int x, int y) {
    camera.move(direction * 2.0f);
}

void Input::launch() {
    trail.clear();
    ball.position = glm::vec3(startPosX, startPosY, startPosZ);
    glm::vec3 velocity(velocityX, velocityY, velocityZ);
    glm::vec3 spin = getCurrentSpin();
    std::cout << "Launching with RPM: " << spinRPM << ", axis: (" << spinAxisX << "," << spinAxisY << "," << spinAxisZ << ")" << std::endl;
    std::cout << "Spin vector: (" << spin.x << "," << spin.y << "," << spin.z << ")" << std::endl;
    simulation.launchWithVelocity(velocity, spin);
}

void Input::reset() {
    trail.clear();
    simulation.reset();
}
