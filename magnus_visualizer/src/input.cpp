#include "input.h"
#include <glm/glm.hpp>

// Default values: typical soccer free kick
// Velocity: 35 m/s forward, 4 m/s up, 8 m/s sideways
// Spin: 700 RPM with slight tilt (-0.28 Z) for curve
Input::Input(Ball& b, Simulation& sim, Camera& cam, Vectors& v, Trail& t) 
    : ball(b), simulation(sim), camera(cam), vectors(v), trail(t),
      velocityX(35.0f), velocityY(4.0f), velocityZ(8.0f),
      spinRPM(700.0f), spinAxisX(0.0f), spinAxisY(1.0f), spinAxisZ(-0.28f),
      startPosX(0.0f), startPosY(0.11f), startPosZ(0.0f) {
}

// Convert RPM and axis to angular velocity in rad/s
glm::vec3 Input::getCurrentSpin() const {
    if (spinRPM == 0.0f) return glm::vec3(0.0f);
    float rpmToRadSec = 2.0f * 3.14159f / 60.0f;
    glm::vec3 axis(spinAxisX, spinAxisY, spinAxisZ);
    float axisLen = glm::length(axis);
    if (axisLen < 0.001f) return glm::vec3(0.0f);
    return glm::normalize(axis) * spinRPM * rpmToRadSec;
}

// Handle keyboard input
void Input::handleKeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': camera.toggleMouseLook(); break;  // Space: toggle mouse look
        case 'r': case 'R': reset(); break;         // R: reset simulation
        case 'v': case 'V': vectors.enabled = !vectors.enabled; break; // V: toggle vectors
        case 't': case 'T': trail.enabled = !trail.enabled; break;     // T: toggle trails
    }
}

// Arrow keys: camera movement (legacy, WASD preferred)
void Input::handleSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    camera.move(0.5f); break;
        case GLUT_KEY_DOWN:  camera.move(-0.5f); break;
        case GLUT_KEY_LEFT:   camera.strafe(0.5f); break;
        case GLUT_KEY_RIGHT:  camera.strafe(-0.5f); break;
    }
}

// Mouse drag: camera rotation
void Input::handleMouseDrag(int dx, int dy) {
    camera.handleMouseMove(dx, dy);
}

// Mouse wheel: zoom (move forward/backward)
void Input::handleMouseWheel(int wheel, int direction, int x, int y) {
    camera.move(direction * 2.0f);
}

// Launch ball with current UI values
void Input::launch() {
    trail.clear();
    ball.position = glm::vec3(startPosX, startPosY, startPosZ);
    glm::vec3 velocity(velocityX, velocityY, velocityZ);
    glm::vec3 spin = getCurrentSpin();
    simulation.launchWithVelocity(velocity, spin);
}

// Reset simulation and clear trails
void Input::reset() {
    trail.clear();
    simulation.reset();
}
