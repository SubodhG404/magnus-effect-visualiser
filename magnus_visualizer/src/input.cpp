#include "input.h"
#include <glm/glm.hpp>

Input::Input(Ball& b, Simulation& sim, Camera& cam, Vectors& v, Trail& t) 
    : ball(b), simulation(sim), camera(cam), vectors(v), trail(t),
      initialSpeed(10.0f), velocityX(10.0f), velocityY(0.0f), velocityZ(0.0f), 
      angularVelocityScale(50.0f), currentSpinType(SpinType::BACKSPIN) {
}

glm::vec3 Input::getCurrentSpin() const {
    switch (currentSpinType) {
        case SpinType::BACKSPIN:
            return glm::vec3(0.0f, 0.0f, angularVelocityScale);
        case SpinType::TOPSPIN:
            return glm::vec3(0.0f, 0.0f, -angularVelocityScale);
        case SpinType::SIDESPIN:
            return glm::vec3(0.0f, angularVelocityScale, 0.0f);
        case SpinType::NOSPIN:
        default:
            return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void Input::handleKeyPress(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            launch();
            break;
        case 'w':
        case 'W':
            initialSpeed += 1.0f;
            break;
        case 's':
        case 'S':
            initialSpeed -= 1.0f;
            if (initialSpeed < 1.0f) initialSpeed = 1.0f;
            break;
        case 'q':
        case 'Q':
            angularVelocityScale += 2.0f;
            break;
        case 'e':
        case 'E':
            angularVelocityScale -= 2.0f;
            if (angularVelocityScale < 0.0f) angularVelocityScale = 0.0f;
            break;
        case '1':
            currentSpinType = SpinType::TOPSPIN;
            break;
        case '2':
            currentSpinType = SpinType::BACKSPIN;
            break;
        case '3':
            currentSpinType = SpinType::SIDESPIN;
            break;
        case '4':
            currentSpinType = SpinType::NOSPIN;
            break;
        case 'm':
        case 'M':
            ball.mass += 0.05f;
            break;
        case 'n':
        case 'N':
            ball.mass -= 0.05f;
            if (ball.mass < 0.1f) ball.mass = 0.1f;
            break;
        case 'v':
        case 'V':
            vectors.toggle();
            break;
        case 'r':
        case 'R':
            reset();
            break;
        case 'a':
        case 'A':
            camera.orbit(-5.0f, 0.0f);
            break;
        case 'd':
        case 'D':
            camera.orbit(5.0f, 0.0f);
            break;
        case 'c':
        case 'C':
            camera.orbit(0.0f, 5.0f);
            break;
        case 'f':
        case 'F':
            camera.orbit(0.0f, -5.0f);
            break;
    }
}

void Input::handleSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camera.pan(0.0f, -0.5f);
            break;
        case GLUT_KEY_DOWN:
            camera.pan(0.0f, 0.5f);
            break;
        case GLUT_KEY_LEFT:
            camera.pan(-0.5f, 0.0f);
            break;
        case GLUT_KEY_RIGHT:
            camera.pan(0.5f, 0.0f);
            break;
    }
}

void Input::handleMouseDrag(int dx, int dy) {
    camera.orbit(dx * 0.5f, -dy * 0.5f);
}

void Input::handleMouseWheel(int wheel, int direction, int x, int y) {
    if (direction > 0) {
        camera.zoom(-1.0f);
    } else {
        camera.zoom(1.0f);
    }
}

void Input::launch() {
    trail.clear();
    glm::vec3 velocity(velocityX, velocityY, velocityZ);
    simulation.launchWithVelocity(velocity, getCurrentSpin());
}

void Input::reset() {
    trail.clear();
    simulation.reset();
}
